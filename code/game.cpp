#include"stdafx.h"
#include"Socu.h"

Equipment ::~Equipment()
{
	auto item = MakeStrongPtr(_item);
	if (item)
		BattleField::inst().removeGameObject(item);
}


class Helmet : public Equipment
{
public:
	Helmet(EntityStrongPtr helmet) : Equipment(helmet) {}
	virtual void attach(EntityStrongPtr player) override;
	virtual void detach() override;

private:
	EntityWeakPtr _player;
};

class Weapon : public Equipment
{
public:
	Weapon(EntityStrongPtr weapon) : Equipment(weapon) {}
	virtual void attach(EntityStrongPtr player) override;
	virtual void detach() override;
private:
	EntityWeakPtr _player;
};

class ItemFactory
{
public:
	static std::shared_ptr<Equipment> create(const ItemData& item)
	{
		std::shared_ptr<Equipment> res;
		auto equipmentEntity = BattleField::inst().spawnObject(item._itemName);
		switch (item._type)
		{
			case EquipmentType::WEAPON:  res.reset(new Weapon(equipmentEntity));
				break;
			case EquipmentType::HELMET:  res.reset(new Helmet(equipmentEntity));
				break;
			default:
				 
		}
		if (res)
		{
			res->_itemID = item._id;
		}
		return res;
	}
};

void Inventory::InventoryView::attachEquipment(EquipmentType type, const ItemData *item)
{
	unsigned index = (unsigned)type;
	if (_equipments[index])
	{
		_equipments[index]->detach();
	}
	// instantiate
	if (item)
	{
		auto euipmentInstance = ItemFactory::create(*item);
		// attach
		auto player = _inventory.getOwnerStrongPtr();
		if (player)
		{
			euipmentInstance->attach(player);
			_equipments[index] = euipmentInstance;
		}
	}
	else
	{
		_equipments[index] = 0x0;
	}
}

void Player::update(float deltams)
{
	bool isWalk = false;
	Vec3f walkdir(0, 0, 0);

	if (FW::Keyboard::isKeyPressed(FW::Keyboard::Left))
	{
		walkdir.x = -_walkspeed;
		isWalk = true;
	}
	else if (FW::Keyboard::isKeyPressed(FW::Keyboard::Right))
	{
		walkdir.x = _walkspeed;
		isWalk = true;
	}
	else if (FW::Keyboard::isKeyPressed(FW::Keyboard::Up))
	{
		walkdir.z = _walkspeed;
		isWalk = true;
	}
	else if (FW::Keyboard::isKeyPressed(FW::Keyboard::Down))
	{
		walkdir.z = -_walkspeed;
		isWalk = true;
	}

	auto owner = getOwnerStrongPtr();			 
	if (owner)
	{
		auto transform = owner->getComponent<Transform>();
		if (transform)
		{

			auto body = owner->getComponent<PhysicsComponent>();
			if (body)
			{
				body->setPos(transform->getPos() + walkdir * deltams);
			}
			else
			{
				transform->setPos(transform->getPos() + walkdir * deltams);
			}
		}
	}

	if (isWalk)
	{
		setState(PlayerState::WALK);
	}
	else
	{
		setState(PlayerState::IDLE);
	}

	 
	if (FW::Keyboard::isKeyPressed(FW::Keyboard::X))
	{
		setState(PlayerState::ATTACK);
	}



	auto animator = owner->getOrCreateComponent<Animator>();
	if (animator)
	{
		switch (_state)
		{
		case PlayerState::IDLE:
			animator->setAnimation("./Animations/Warrior_Idle.anim");
			break;
		case PlayerState::WALK:
			animator->setAnimation("./Animations/Warrior_Walk.anim");
			break;
		case PlayerState::ATTACK:
			animator->setAnimation("./Animations/Warrior_Attack.anim");
			break;
		}
	}

	if (_state == PlayerState::ATTACK)
	{
		auto mobs = BattleField::inst().getAllMobs();
		for (auto &mob : mobs)
		{
			if (mob->getComponent<Transform>() &&
				owner->getComponent<Transform>())
			{
				auto dis = (mob->getComponent<Transform>()->getAbsPos() - owner->getComponent<Transform>()->getAbsPos()).abs();
				if (dis < 10)
				{
					if (mob->getComponent<Mob>())
					{
						auto mobcmp = mob->getComponent<Mob>();
						mobcmp->onAttack();
					}
				}
			}
		}
	}
}


void Mob::update(float deltaSeconds)
{
	auto owner = getOwnerStrongPtr();
	if (_health <= 0)
	{
		BattleField::inst().removeGameObject(owner);

		 
		auto equipment = BattleField::inst().spawnEquipment();
		if (equipment && equipment->getComponent<Transform>())
		{
			if (equipment->getComponent<PhysicsComponent>())
			{
				equipment->getComponent<PhysicsComponent>()->setPos(owner->getComponent<Transform>()->getAbsPos());
			}
			else
			{
				equipment->getComponent<Transform>()->setPos(owner->getComponent<Transform>()->getAbsPos());
			}
			equipment->getComponent<Transform>()->setScale(Vec3f(0.1f, 0.1f, 0.1f));
		}
	}
}
