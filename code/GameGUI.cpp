#include"stdafx.h"
#include"GameUI.h"
#include"Socu.h"

EquipmentSlot::EquipmentSlot(std::shared_ptr<PlayerInfo> playerInfo, EquipmentType type) :
	_playerInfo(playerInfo),
	_type(type)
{
	onMouseRightClick = [this]()
	{
		auto player = BattleField::inst().getPlayer();
		if (player)
		{
			auto inventory = player->getComponent<Inventory>();
			inventory->addItem(inventory->getEquipmentIDByType(_type));
			inventory->detachEquipment(_type);
		}
	};
}

void EquipmentSlot::draw()
{
	auto player = BattleField::inst().getPlayer();
	if (player)
	{
		auto inventory = player->getComponent<Inventory>();
		if (inventory)
		{
			auto item = EquipmentDB::inst().getItemByID(inventory->getEquipmentIDByType(_type));
			if (item)
			{
				GUI::Panel::setTexture(item->_iconTextureName);
				GUI::Panel::draw();
			}
		}
	}
}

InventorySlot::InventorySlot(std::shared_ptr<PlayerInfo> playerInfo, unsigned index) :
	_playerInfo(playerInfo),
	_slotPosition(index)
{
	
	onMouseRightClick = [this]()
	{
		auto player = BattleField::inst().getPlayer();
		if (player)
		{
			auto inventory = player->getComponent<Inventory>();
			auto itemID = inventory->getItemByPosition(this->_slotPosition);
			auto item = EquipmentDB::inst().getItemByID(itemID);
			if (item)
			{
				inventory->addItem(inventory->getEquipmentIDByType(item->_type));
				inventory->detachEquipment(item->_type);
				inventory->attachEquipment(item->_type, item);
				inventory->removeItemByPosition(this->_slotPosition);
			}
		}
	};
}

void InventorySlot::draw()
{
	auto player = BattleField::inst().getPlayer();
	if (player)
	{
		auto inventory = player->getComponent<Inventory>();
		auto itemID = inventory->getItemByPosition(this->_slotPosition);
		auto item = EquipmentDB::inst().getItemByID(itemID);
		if (item)
		{
			GUI::Panel::setTexture(item->_iconTextureName);
			GUI::Panel::draw();
		}
	}
}

SelectLevelUI::SelectLevelUI()
{
	  
	onMouseLeftClick = []()
	{
		Socu::inst().unLoadCurrentLevel();
		std::string scenefile = g_FileSystem->GetProgramDir() + "/MapData/aa.xml";
		Socu::inst().loadLevel(scenefile);
	};
}
