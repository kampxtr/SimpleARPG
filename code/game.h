#pragma once
#include"GameUI.h"

class Equipment
{
public:
	Equipment(EntityStrongPtr item) : _item(item) {}
	virtual ~Equipment();
	
	virtual void attach(EntityStrongPtr player)
	{
		auto item = MakeStrongPtr(_item); 
		if (item)
		{
			if (player->getComponent<Transform>() && item->getComponent<Transform>())
			{
				player->getComponent<Transform>()->addChild(item->getComponent<Transform>());
			}
 
			auto body = item->getComponent<PhysicsComponent>();
			if (body)
			{
				body->enable(false);
			}
		}
	}
	virtual void detach() {}

public:
	unsigned _itemID;
protected:
	EntityWeakPtr _item;
};

class ItemData
{
public:
	unsigned _id;
	EquipmentType _type;
	std::string _itemName;
	std::string _iconTextureName;
};

class EquipmentDB
{
public:
	static EquipmentDB& inst()
	{
		static EquipmentDB* s_db = 0x0;
		if (!s_db) s_db = new EquipmentDB;
		return *s_db;
	}
	bool init()
	{
		_items.push_back({ 0, EquipmentType::WEAPON, WEAPONMODEL, "Textures/Sword_03_diffuse.dds" });
		_items.push_back({ 1, EquipmentType::HELMET, HELMETMODEL, "Textures/Helmet03_03_diffuse.dds" });
		return true;
	}
	ItemData randItem() const
	{
		int id = rand() % _items.size();
		return _items[id];
	}
	const ItemData* getItemByID(unsigned id) const
	{
		if (0 <= id && id < _items.size())
		{
			return &_items[id];
		}
		return 0x0;
	}
private:
	std::vector<ItemData>	_items;
};

class GroundItem : public Component
{
public:
	DECLARE_SIMPLE_COMPONENT(GroundItem)
	unsigned _itemID;
};
 
class Inventory : public Component
{
public:
	DECLARE_SIMPLE_COMPONENT(Inventory)
	Inventory()
	{
	}

	bool init(std::shared_ptr<PlayerInfo> playerInfo)
	{
		_inventoryLogic.reset(new InventoryLogic(*this, playerInfo));
		if (!_inventoryLogic.get() || !_inventoryLogic->init())
			return false;
		_inventoryView.reset(new InventoryView(*this, _inventoryLogic.get()));
		if (!_inventoryView.get() || !_inventoryView->init())
			return false;
		return true;
	}

	void addItem(unsigned itemID)
	{
		_inventoryLogic->addItem(itemID);
		_inventoryView->addItem(itemID);
	}

	void removeItemByPosition(unsigned position)
	{
		_inventoryLogic->removeItemByPosition(position);
		_inventoryView->removeItemByPosition(position);
	}

	unsigned getItemByPosition(unsigned i)
	{
		return _inventoryLogic->getItemByPosition(i);
	}

	void attachEquipment(EquipmentType type, const ItemData *item)
	{
		_inventoryLogic->attachEquipment(type, item);
		_inventoryView->attachEquipment(type, item);
	}

	void detachEquipment(EquipmentType type)
	{
		_inventoryLogic->detachEquipment(type);
		_inventoryView->detachEquipment(type);
	}

	unsigned getEquipmentIDByType(EquipmentType type)
	{
		return _inventoryLogic->getEquipmentIDByType(type);
	}

	class InventoryLogic
	{
	public:
		InventoryLogic(Inventory &inventory, std::shared_ptr<PlayerInfo> playerInfo) :
			_inventory(inventory), 
			_playerInfo(playerInfo)
		{
		}

		bool init()
		{
			return true;
		}

		void addItem(unsigned itemID)
		{
			_playerInfo->addItem(itemID);
		}

		void removeItemByPosition(unsigned position)
		{
			_playerInfo->removeItemByPosition(position);
		}

		unsigned getItemByPosition(unsigned i)
		{
			return _playerInfo->getItemByPosition(i);
		}

		unsigned getEquipmentIDByType(EquipmentType type)
		{
			return _playerInfo->getEquipmentIDByType(type);
		}

		void attachEquipment(EquipmentType type, const ItemData *item)
		{
			unsigned index = (unsigned)type;
			if (item)
				_playerInfo->attachEquipment(type, item->_id);
			else
				_playerInfo->detachEquipment(type);
		}

		void detachEquipment(EquipmentType type)
		{
			_playerInfo->detachEquipment(type);
		}
		
		std::shared_ptr<PlayerInfo> getPlayerInfo() const
		{
			return _playerInfo;
		}
	private:
		Inventory &_inventory;
		std::shared_ptr<PlayerInfo> _playerInfo;
	};

	class InventoryView
	{
	public:
		InventoryView(Inventory &inventory, InventoryLogic* logic) :
			_inventory(inventory),
			_logic(logic)
		{
		}

		~InventoryView()
		{
		}
		
		bool init()
		{
			if (!_logic)
				return false;
			
			auto playerInfo = _logic->getPlayerInfo();
			if (!playerInfo)
				return false;
			for(unsigned i = 0; i < playerInfo->_equipments.size(); ++ i)
			{
				auto item = EquipmentDB::inst().getItemByID(playerInfo->_equipments[i]);
				attachEquipment((EquipmentType)i, item);
			}

			return true;
		}

		void addItem(unsigned itemID) { }

		void removeItemByPosition(unsigned position) { }

		void attachEquipment(EquipmentType type, const ItemData *item);
		

		void detachEquipment(EquipmentType type)
		{
			auto index = (unsigned)type;
			if (_equipments[index])
				_equipments[index]->detach();
			_equipments[index] = 0x0;
		}

	private:
		Inventory &_inventory;
		InventoryLogic *_logic;
		std::array<std::shared_ptr<Equipment>, (unsigned)EquipmentType::MAX> _equipments;
	};
public:
	std::unique_ptr<InventoryLogic> _inventoryLogic;
	std::unique_ptr<InventoryView> _inventoryView;
};

enum class PlayerState
{
	IDLE,
	WALK,
	ATTACK,
}; 

class Player : public Component
{
public:
	DECLARE_SIMPLE_COMPONENT(Player)

	void update(float deltams);

	void setState(PlayerState newstate)
	{
		_state = newstate;
	}

	float _walkspeed = 10.2f;
	PlayerState _state;
};

class Mob : public Component
{
public:
	DECLARE_SIMPLE_COMPONENT(Mob)
	void onAttack()
	{
		_health -= 55;
	}

	void update(float deltaSeconds);
	int _health = 100;
};

enum class GameObjectTypes
{
	NONE,
	PLAYER,
	MOB,
	EQUIPMENT,
};
class GameObject : public Component
{
public:
	DECLARE_SIMPLE_COMPONENT(GameObject)
	GameObjectTypes _type;
};

class BattleField
{
public:
	static BattleField& inst()
	{
		static BattleField *s_bf = 0x0;
		if (!s_bf)
		{
			s_bf = new BattleField;
		}
		return *s_bf;
	}
	BattleField()
	{
	}

	~BattleField()
	{
	}

	bool init(std::shared_ptr<Scene> scene)
	{
		_scene = scene.get();
		spawnMobs();
		return true;
	}

	bool close()
	{
		return true;
	}

	EntityStrongPtr spawnObject(const std::string &name);
	 
	EntityStrongPtr getPlayer();

	EntityStrongPtr spawnPlayer(std::shared_ptr<PlayerInfo> playerInfo)
	{
		auto player = spawnObject(playerInfo->_name);
		_myCopy = player;
		if (player)
		{
			player->getComponent<GameObject>()->_type = GameObjectTypes::PLAYER;
			player->getOrCreateComponent<Player>();
			player->getOrCreateComponent<Inventory>()->init(playerInfo);
		}

		return player;
	}

	EntityStrongPtr spawnMob(const std::string &name)
	{
		auto mob = spawnObject(name);
		if (mob)
		{
			mob->getComponent<GameObject>()->_type = GameObjectTypes::MOB;
			mob->getOrCreateComponent<Mob>();
		}
		return mob;
	}

	EntityStrongPtr spawnEquipment()
	{
		ItemData item = EquipmentDB::inst().randItem();
		auto euipment = spawnObject(item._itemName);
		if (euipment)
		{
			euipment->getComponent<GameObject>()->_type = GameObjectTypes::EQUIPMENT;
			auto grounditem = euipment->getOrCreateComponent<GroundItem>();
			if (grounditem)
				grounditem->_itemID = item._id;
		}
		return euipment;
	}

	void spawnMobs()
	{
		
			for (int i = 0; i < 30; ++i)
			{
				auto mob = spawnMob(PENGUINMODEL);
				auto transform = mob->getComponent<Transform>();
				if (transform)
				{
					//transform->setPos(Vec3f(150 * cos(i), 10, 150 * sin(i)));
					transform->setPos(
						Vec3f(cos(rand() % 10000) * 100, 2.4, sin(rand() % 10000) * 10));
					transform->setRot(Quat(Vec3f(0, 1, 0), 10.f * i * pi / 180));
				}
			}
		
	}

	void removeGameObject(EntityStrongPtr entity);

	void update(float deltams)
	{
		if (_scene)
		{
			for (auto &go : _scene->iteraotr())
			{
				auto gocmp = go->getComponent<GameObject>();
				if (gocmp)
				{
					switch (gocmp->_type)
					{
					case GameObjectTypes::PLAYER:  go->getComponent<Player>()->update(deltams);
						break;
					case GameObjectTypes::MOB: go->getComponent<Mob>()->update(deltams);
						break;
					case GameObjectTypes::EQUIPMENT:break;

					}
				}
			}
		}

	}

	std::list<EntityStrongPtr> getAllMobs() const
	{
		std::list<EntityStrongPtr> mobs;
		for (auto &go : _scene->iteraotr())
		{
			auto gocmp = go->getComponent<GameObject>();
			if (gocmp) 
			{
				switch (gocmp->_type)
				{ 
				case GameObjectTypes::MOB: mobs.push_back(go);
					break;
				}
			}
		}
		return mobs;
	}

	void onTrggerIn(IEventDataPtr pData)
	{
		auto evt = std::dynamic_pointer_cast<PhysicsEvent_TriggerIn>(pData);
		EntityStrongPtr trigger = _scene->getEntityById(evt->GetTriggerId());
		EntityStrongPtr go = _scene->getEntityById(evt->GetOtherActor());
		handleTriggerIn(trigger, go);
	}
	
	void handleTriggerIn(EntityStrongPtr trigger, EntityStrongPtr go)
	{
		if (trigger)
		{
			auto gocmp = trigger->getComponent<GameObject>();
			if (gocmp->_type == GameObjectTypes::EQUIPMENT)
			{
				auto body = trigger->getComponent<PhysicsComponent>();
				if (body)
					body->setMass(0);

				if (go)
				{
					if (go->getComponent<GameObject>() && 
						go->getComponent<GameObject>()->_type == GameObjectTypes::PLAYER)
					{
						go->getComponent<Inventory>()->addItem(trigger->getComponent<GroundItem>()->_itemID);
						removeGameObject(trigger);
					}
				}

			}
		}
	}

private:
	Scene* _scene;
	EntityWeakPtr _myCopy;
};

class Socu
{
public:
	Socu()  : 
		_bf(0x0)
	{
	} 
	~Socu()
	{
		
	}
	static Socu& inst()
	{
		static Socu* s_game = 0x0;
		if (!s_game) s_game = new Socu;
		return *s_game;
	}
	bool init()
	{
		registerEvents();
		registerDelegates();

		onGameStart();
		return true;
	}
	
	bool close()
	{
		onGameEnd();
		if (_bf)
		{
			if (!_bf->close())
				return false;
		}
		unRegisterDelegates();
		return true;
	}

	// game-level main loop
	void update(IEventDataPtr e)
	{
		auto updatee = std::dynamic_pointer_cast<UpdateEvent>(e);
		float deltams = updatee->_deltams;

		// battle logic
		if(_bf)
			_bf->update(deltams);
		_playerInventoryGUI->update(deltams);

		// physics
		auto physics = Engine::physics();
		if(physics)
		{
			physics->update(deltams);
			physics->syncScene(_scene.get());
		}

		// animations
		auto animSys = Engine::animationSystem();
		if (animSys)
		{
			animSys->update(deltams);
		}
	}

	BattleField* getModule() const { return _bf; }


	bool loadLevel(const std::string &path)
	{
		// auto scene = new Scene
		_scene = SceneHandler::inst().createScene("game scene");
		SceneHandler::inst().setCurScene(_scene);
		if (_scene)
		{
			XMLDoc scenexml;
			if (scenexml.parseFile(path.c_str()))
			{
				_scene->fromXml(scenexml); 
				_bf = &BattleField::inst();
				if (!_bf->init(_scene))
					return true;
				_bf->spawnPlayer(_playerInfo);
			}
			else
				return false;
		}
		else
			return false;
	}

	void unLoadCurrentLevel()
	{
		SceneHandler::inst().removeScene(_scene);
	}


	bool onGameStart()
	{
		_playerInfo.reset(new PlayerInfo);
		_playerInfo->load();


		// load game data
		if (!EquipmentDB::inst().init())
			return false;


		_selectLevelGUI.reset(new SelectLevelUI);
		UIHandler::inst().add(_selectLevelGUI);

		_playerInventoryGUI.reset(new PlayerInventoryGUI(_playerInfo));
		if (!_playerInventoryGUI->init())
			return false;
		UIHandler::inst().add(_playerInventoryGUI);
		
		return true;
	}

	bool onGameEnd()
	{
		unLoadCurrentLevel();
		return true;
	}
 

private:
	std::shared_ptr<Scene> _scene;
	BattleField *_bf;
	std::shared_ptr<PlayerInfo> _playerInfo;
	std::shared_ptr<PlayerInventoryGUI> _playerInventoryGUI;
	std::shared_ptr<SelectLevelUI> _selectLevelGUI;
};
