#pragma once
#include "wbpch.h"
#include "ECSEntity.h"
#include "ECSComponent.h"

#include "Events/EventBus.h"
#include "Logger/Logger.h"

namespace Workbench {

	class AbstractComponentContainer {};

	template<class Component>
	class ComponentContainer : public AbstractComponentContainer {
	public:

		const UUID* AddComponent(Component* component) {
			for (auto ex_component : m_Components)
				if (ex_component == component) {
					auto _ptr = static_cast<ECSComponent*>(ex_component);
					return _ptr->getUuid();
				}
			m_Components.push_back(component);
			auto _ptr = static_cast<ECSComponent*>(component);
			return _ptr->getUuid();
		}

		bool RemoveComponent(Component* component) {
			auto it = m_Components.begin();
			auto end = m_Components.end();
			for (; it != end; ++it) {
				if ((*it) == component) {
					m_Components.erase(it);
					return true;
				}
			}
			return false;
		}

		std::pair<typename std::vector<Component*>::iterator, typename std::vector<Component*>::iterator> getComponents() {
			return { m_Components.begin(), m_Components.end() };
		}

		const Component* getComponentById(const UUID* componentId) {
			for (auto component : m_Components) {
				if (static_cast<ECSComponent*>(component)->getUuid() == componentId)
					return component;
			}
			return nullptr;
		}

	protected:
		std::vector<Component*> m_Components;
	};

	class ECS {
	public:
		EMITS_EVENTS {
			EntityCreatedEvent = 0,
			EntityDestroyedEvent

		};

		class EntityCreatedEvent EVENT {
			SET_EVENT_TYPE(EntityCreatedEvent)
		public:
			EntityCreatedEvent(const UUID* entitiId) : m_EntityId(entitiId) {};
			const UUID* Entity() { return m_EntityId; };
		protected:
			const UUID* m_EntityId;
		};

		class EntityDestroyedEvent EVENT {
			SET_EVENT_TYPE(EntityDestroyedEvent)
		public:
			EntityDestroyedEvent(const UUID* entitiId) : m_EntityId(entitiId) {};
			const UUID* Entity() { return m_EntityId; };
		protected:
			const UUID* m_EntityId;
		};

		static ECS* getInstance();

		const UUID* CreateEntity();
		void DestroyEntity(const UUID* entityId);

		template<class Component>
		void AddComponent(const UUID* entityId, Component* component) {
			//Check for component validity
			static_assert(std::is_base_of<ECSComponent, Component>::value, "Component class should be derived from ECSComponent!");

			//If there is no container yet, create one
			if (m_ComponentContainers[typeid(Component)] == nullptr)
				m_ComponentContainers[typeid(Component)] = static_cast<AbstractComponentContainer*>(new ComponentContainer<Component>);

			//Type cast to desired container
			ComponentContainer<Component>* componentContainer = (ComponentContainer<Component>*)(m_ComponentContainers[typeid(Component)]);

			//Add component
			auto component_id = componentContainer->AddComponent(component);
			m_EntityToComponentsMap[entityId][typeid(Component)].push_back(component_id);
		};

		template<class Component>
		void RemoveComponent(const UUID* entityId, Component* component) {
			//type cast to desired container
			ComponentContainer<Component>* componentContainer = (ComponentContainer<Component>*)(m_ComponentContainers[typeid(Component)]);

			if (componentContainer->RemoveComponent(component)) {
				auto it = m_EntityToComponentsMap[entityId][typeid(Component)].begin();
				auto end = m_EntityToComponentsMap[entityId][typeid(Component)].end();
				for (; it != end; it++) {
					if ((*it) == (static_cast<ECSComponent*>(component))->getUuid()) {
						m_EntityToComponentsMap[entityId][typeid(Component)].erase(it);
						break;
					}
				}
			}
		};
		
		template<class ...Components>
		std::vector<const UUID*>::iterator GetEntities();
		
		template<class Component>
		const Component* GetEntityComponent(const UUID* entityId) const {
			ComponentContainer<Component>* compContainer = static_cast<ComponentContainer<Component>*>(m_ComponentContainers[typeid(Component)]);
			if (m_EntityToComponentsMap[entityId][typeid(Component)].empty())
				return nullptr;
			else
				return compContainer->getComponentById(m_EntityToComponentsMap[entityId][typeid(Component)][0]);
		};
		
		template<class Component>
		std::pair<typename std::vector<Component*>::iterator, typename std::vector<Component*>::iterator> GetComponents() {
			ComponentContainer<Component>* compContainer = static_cast<ComponentContainer<Component>*>(m_ComponentContainers[typeid(Component)]);
			return compContainer->getComponents();
		};
	protected:

		ECS() = default;

		std::vector<std::unique_ptr<ECSEntity>> m_Entities;

		std::map<std::type_index, AbstractComponentContainer*> m_ComponentContainers;

		std::map<const UUID*, std::map<std::type_index, std::vector<const UUID*>>> m_EntityToComponentsMap;

	protected:
		static ECS* m_instance;
	};
}
