#pragma once
#include "wbpch.h"
#include "ECSEntity.h"
#include "ECSComponent.h"

#include "Events/Event.h"
#include "Logger.h"

namespace Workbench {
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

		const UUID* CreateEntity();
		void DestroyEntity(const UUID* entityId);

		template<class Component>
		void AddComponent(const UUID* entityId, std::shared_ptr<const Component> component) {
			if (m_ComponentContainers[typeid(Component)] == nullptr)
				m_ComponentContainers[typeid(Component)] = static_cast<AbstractComponentContainer*>(new ComponentContainer<Component>);
			ComponentContainer<Component>* componentContainer = (ComponentContainer<Component>*)(m_ComponentContainers[typeid(Component)]);

			auto component_id = componentContainer->AddComponent(component);
			m_EntityToComponentsMap[entityId][typeid(Component)].push_back(component_id);
		};

		template<class Component>
		void RemoveComponent(const UUID* entityId, std::shared_ptr<const Component> component) {
			ComponentContainer<Component>* componentContainer = (ComponentContainer<Component>*)(m_ComponentContainers[typeid(Component)]);
			if (componentContainer->RemoveComponent(component)) {
				auto it = m_EntityToComponentsMap[entityId][typeid(Component)].begin();
				auto end = m_EntityToComponentsMap[entityId][typeid(Component)].end();
				for (; it != end; it++) {
					if ((*it) == (std::static_pointer_cast<const ECSComponent>(component))->getUuid())
						m_EntityToComponentsMap[entityId][typeid(Component)].erase(it);
				}
				
			}
		};

		template<class Component>
		void Test(const UUID* entityId) {
			ComponentContainer<Component>* componentContainer = (ComponentContainer<Component>*)(m_ComponentContainers[typeid(Component)]);

			auto [begin, end] = componentContainer->getComponents();

			WB_CORE_INFO("sharead_ptr test: {}", (*begin).use_count());
		}
		
		template<class ...Components>
		std::vector<const UUID*>::iterator GetEntities();
		
		template<class Component>
		typename std::vector<const Component*>::iterator GetEntityComponent(const UUID* entityId);
		
		template<class Component>
		typename std::vector<const Component*>::iterator GetComponents() {
			//return 
		};
	protected:

		class AbstractComponentContainer {};

		template<class Component>
		class ComponentContainer : public AbstractComponentContainer {
		public:
			typedef std::shared_ptr<const Component> ComponentPtr;

			const UUID* AddComponent(ComponentPtr component) {
				for (auto ex_component : m_Components)
					if (ex_component == component) {
						std::shared_ptr<const ECSComponent> _ptr = std::static_pointer_cast<const ECSComponent>(ex_component);
						return _ptr->getUuid();
					}
				m_Components.push_back(component);
				std::shared_ptr<const ECSComponent> _ptr = std::static_pointer_cast<const ECSComponent>(component);
				return _ptr->getUuid();
			}

			bool RemoveComponent(ComponentPtr component) {
				auto it = m_Components.end();
				auto begin = m_Components.begin();
				for (; it != begin; it--) {
					if ((*it) == component) {
						m_Components.erase(it);
						return true;
					}
				}
				return false;
			}

			std::pair<typename std::vector<ComponentPtr>::iterator, typename std::vector<ComponentPtr>::iterator> getComponents() { 
				return { m_Components.begin(), m_Components.end() }; 
			}

		protected:
			std::vector<ComponentPtr> m_Components;
		};

		std::vector<std::unique_ptr<ECSEntity>> m_Entities;

		std::map<std::type_index, AbstractComponentContainer*> m_ComponentContainers;

		std::map<const UUID*, std::map<std::type_index, std::vector<const UUID*>>> m_EntityToComponentsMap;
	};
}

#define CREATE_ECS_COMPONENT(ComponentType, ...) std::shared_ptr<const ComponentType>(new ComponentType(__VA_ARGS__))

