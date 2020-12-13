#pragma once
#include "wbpch.h"
#include "Core.h"
#include "ECS/ECSEntity.h"
#include "ECS/ECSComponent.h"
#include "ECS/ECSComponentContainer.h"

#include "Events/EventBus.h"
#include "Logger/Logger.h"

namespace Workbench {

	class WORKBENCH_API ECS {
	public:
		EMITS_EVENTS {
			EntityCreatedEvent = 0,
			EntityComponentsChangedEvent,
			EntityDestroyedEvent

		};

		class EntityCreatedEvent EVENT {
			SET_EVENT_TYPE(EntityCreatedEvent)
		public:
			EntityCreatedEvent(const UUID* entitiId) : m_EntityId(entitiId) {};
			const UUID* Entity() const { return m_EntityId; };
		protected:
			const UUID* m_EntityId;
		};

		class EntityDestroyedEvent EVENT {
			SET_EVENT_TYPE(EntityDestroyedEvent)
		public:
			EntityDestroyedEvent(const UUID* entitiId) : m_EntityId(entitiId) {};
			const UUID* Entity() const { return m_EntityId; };
		protected:
			const UUID* m_EntityId;
		};

		class EntityComponentsChangedEvent EVENT {
			SET_EVENT_TYPE(EntityComponentsChangedEvent)
		public:
			enum class ActionType {
				ComponentCreated = 0,
				ComponentChanged,
				ComponentDestroyed
			};

			EntityComponentsChangedEvent(
				ECSComponent* component, 
				std::type_index component_type, 
				ActionType action
			) : m_Component(component), 
				m_ComponentType(component_type), 
				m_ActionType(action) {};

			~EntityComponentsChangedEvent() {
				if(m_ActionType == ActionType::ComponentDestroyed)
					delete m_Component;
			}

			ECSComponent* GetComponent() const { return m_Component; };
			std::type_index GetComponentType() const { return m_ComponentType; };
			ActionType GetActionType() const { return m_ActionType; };
		protected:
			ECSComponent* m_Component;
			std::type_index m_ComponentType;
			ActionType m_ActionType;
		};

		const UUID* CreateEntity();
		void DestroyEntity(const UUID* entityId);

		static ECS* GetInstance();

	public:

		template<class Component, typename ...Args>
		Component* CreateComponentForEntity(const UUID* entityId, Args... args) {
			Component* newComponent = new Component(entityId, args...);

			AddComponent(entityId, newComponent);

			return newComponent;
		}

		template<class Component>
		void AddComponent(const UUID* entityId, Component* component) {
			#ifdef WB_DEBUG
			//Check for component validity
			static_assert(std::is_base_of<ECSComponent, Component>::value, "Component class should be derived from ECSComponent!");
			#endif

			//If there is no container yet, create one
			if (m_ComponentContainers[typeid(Component)] == nullptr)
				m_ComponentContainers[typeid(Component)] = new ECSComponentContainer();

			//Type cast to desired container
			ECSComponentContainer* componentContainer = m_ComponentContainers[typeid(Component)];

			//Add component
			auto component_id = componentContainer->AddComponent(static_cast<ECSComponent*>(component));
			m_EntityToComponentsMap[entityId][typeid(Component)]= component_id;
			POST_EVENT(new EntityComponentsChangedEvent(static_cast<ECSComponent*>(component), typeid(Component), EntityComponentsChangedEvent::ActionType::ComponentCreated));
		}; 

		template<class Component>
		void RemoveComponent(const UUID* entityId, Component* component) {
			//type cast to desired container
			DestroyComponent(entityId, typeid(Component), static_cast<ECSComponent*>(component)->getUuid());
		};

		void DestroyComponent(const UUID* entityId, std::type_index componentType, const UUID* component) {
			ECSComponentContainer* componentContainer = m_ComponentContainers[componentType];

			if (componentContainer) {
				auto _component = componentContainer->RemoveComponent(component);
				if (_component) {
					m_EntityToComponentsMap[entityId].erase(componentType);
					SEND_EVENT(new EntityComponentsChangedEvent(_component, componentType, EntityComponentsChangedEvent::ActionType::ComponentDestroyed));
				}
			}
		}
		
		template<class ...Components>
		std::vector<const UUID*>::iterator GetEntities();
		
		template<class Component>
		Component* GetEntityComponent(const UUID* entityId) {
			ECSComponentContainer* compContainer = m_ComponentContainers[typeid(Component)];
			if (m_EntityToComponentsMap[entityId].find(typeid(Component)) == m_EntityToComponentsMap[entityId].end())
				return nullptr;
			else
				return static_cast<Component*>(compContainer->GetComponentById(m_EntityToComponentsMap[entityId][typeid(Component)]));
		};
		
		template<class Component>
		std::vector<Component*> GetComponents() {
			//If there is no container yet, create one
			if (m_ComponentContainers[typeid(Component)] == nullptr)
				m_ComponentContainers[typeid(Component)] = new ECSComponentContainer();

			ECSComponentContainer* compContainer = m_ComponentContainers[typeid(Component)];
			auto [begin, end] = compContainer->GetComponents();
			std::vector<Component*>* components = reinterpret_cast<std::vector<Component*>*>(compContainer->_GetComponents());
			return *components;
		};

	protected:

		ECS() = default;

		std::vector<std::shared_ptr<ECSEntity>> m_Entities;

		std::map<std::type_index, ECSComponentContainer*> m_ComponentContainers;

		std::map<const UUID*, std::map<std::type_index, const UUID*>> m_EntityToComponentsMap;

	protected:
		static ECS* m_instance;
	};
}

