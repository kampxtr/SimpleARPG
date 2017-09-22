#pragma once 
#include"GameDefine.h" 

class EquipmentSlot : public GUI::Panel
{
public:
	EquipmentSlot(std::shared_ptr<PlayerInfo> playerInfo, EquipmentType type);
	virtual void draw() override;

public:
	static const unsigned ICON_SIZE = 50;
private:
	std::shared_ptr<PlayerInfo> _playerInfo;
	EquipmentType _type;
};

class InventorySlot : public GUI::Panel
{
public:
	InventorySlot(std::shared_ptr<PlayerInfo> playerInfo, unsigned index);
	virtual void draw() override;

public:
	static const unsigned ICON_SIZE = 50;
private:
	std::shared_ptr<PlayerInfo> _playerInfo;
	unsigned _slotPosition;
};
 
class PlayerInventoryGUI : public GUI::GUIContainer
{
public:
	PlayerInventoryGUI(std::shared_ptr<PlayerInfo> playerInfo) :
		_playerInfo(playerInfo),
		_isOpen(false)
	{
	}

	bool init()
	{
		_lastPressFrame = 0;
		try
		{
			 
			auto panel = std::shared_ptr<GUI::Panel>(new GUI::Panel);
			_inventoryWindow = panel;
			panel->SetSizeAllocation(FW::FloatRect(200, 200, _inventoryWindowSize, _inventoryWindowSize));
			panel->setDragable(true);
			panel->setTexture("Textures/inventortbg.dds");
			GUIContainer::addComponent(_inventoryWindow);

			// equipment slots
			for (unsigned i = 0; i < (unsigned)EquipmentType::MAX; ++i)
			{
				auto slot = std::shared_ptr<EquipmentSlot>(new EquipmentSlot(_playerInfo, (EquipmentType)i));
				panel->addComponent(slot);
				int horizon = i % 5 * EquipmentSlot::ICON_SIZE;
				int vertical = i / 5 * EquipmentSlot::ICON_SIZE;
				slot->SetSizeAllocation(FW::FloatRect(200 + horizon, vertical, EquipmentSlot::ICON_SIZE, EquipmentSlot::ICON_SIZE));
			}

			//item slots
			for (unsigned i = 0; i < MAX_INVENTORY_ITEM_COUNT; ++i)
			{
				auto slot = std::shared_ptr<InventorySlot>(new InventorySlot(_playerInfo, i));
				panel->addComponent(slot);
				int horizon = i % 5 * InventorySlot::ICON_SIZE;
				int vertical = i / 5 * InventorySlot::ICON_SIZE;
				slot->SetSizeAllocation(FW::FloatRect(horizon, 200 + vertical, InventorySlot::ICON_SIZE, InventorySlot::ICON_SIZE));
			}
		}
		catch (...)
		{
			 
			return false;
		}
		return true;
	}

	void update(float deltaSecond)
	{
		_lastPressFrame--;
		if (_lastPressFrame < 0) _lastPressFrame = 0;
		if (!_isOpen)
		{
			if (FW::Keyboard::isKeyPressed(FW::Keyboard::I) && _lastPressFrame == 0)
			{
				_isOpen = true;
				_lastPressFrame = 10;
			}
		}
		else
		{
			if (FW::Keyboard::isKeyPressed(FW::Keyboard::I) && _lastPressFrame == 0)
			{
				_isOpen = false;
				_lastPressFrame = 10;
			}
		}
	}

	virtual void draw() override
	{
		if (_isOpen)
		{
			GUI::GUIContainer::draw();
		}
	}
private:
	static const unsigned _inventoryWindowSize = 500;
	std::shared_ptr<GUI::GUIContainer> _inventoryWindow;
	bool _isOpen;
	std::shared_ptr<PlayerInfo> _playerInfo;
	int _lastPressFrame;
};

class SelectLevelUI : public GUI::Panel
{
public:
	SelectLevelUI();
};
