
component = "inventory_view"

local inventoryWindowSize = 500;
local ICON_SIZE = 50;

local playerWindow = {}
local equipmentIcons = {}
local itemIcons = {}


local function onItemChange(type, data) -- data = { pos, itemdata }
	if(itemIcons[data.pos]) then
		if(data.itemdata == INVALID_ITEM) then
			itemIcons[data.pos]:setTexture("Textures/inventortbg.dds")
		else
			itemIcons[data.pos]:setTexture(data.itemdata.icon)
		end
	end
end

local function onEquipmentChange(type, data) -- data = { pos, itemdata }
	if(equipmentIcons[data.pos]) then
		if(data.itemdata == INVALID_ITEM) then
			equipmentIcons[data.pos]:setTexture("Textures/inventortbg.dds")
		else
			equipmentIcons[data.pos]:setTexture(data.itemdata.icon)
		end
	end
end

function start()
	Event:register(game_event.ITEM_DATA_CHANGE, onItemChange)
	Event:register(game_event.EQUIPMENT_DATA_CHANGE, onEquipmentChange)

	playerWindow = GUI.Panel.create()
	playerWindow:root()
	playerWindow:setRect(FW.FloatRect.new(300, 300, inventoryWindowSize, inventoryWindowSize))
	playerWindow:setTexture("Textures/inventortbg.dds")
	playerWindow:setDragable(true)


	for i=0, EQUIPMENT_TYPE.MAX - 1 do
		local icon = GUI.Panel.create() 
		local horizon = math.floor(i % 5) * ICON_SIZE
	    local vertical = math.floor(i / 5) * ICON_SIZE
		icon:setRect(FW.FloatRect.new(200 + horizon, vertical, ICON_SIZE, ICON_SIZE))
		icon.onMouseRightClick = function() 
			this.entity.inventory_ctr.detachEquipment(i + 1)
		end
		playerWindow:addComponent(icon)
		icon:setTexture("Textures/inventortbg.dds")
		equipmentIcons[i + 1] = icon
	end

	for i=0, MAX_INVENTORY_ITEM_COUNT - 1 do
		local icon = GUI.Panel.create() 
		local horizon = math.floor(i % 5) * ICON_SIZE
	    local vertical = math.floor(i / 5) * ICON_SIZE
		icon:setRect(FW.FloatRect.new(horizon, 200 + vertical, ICON_SIZE, ICON_SIZE))
		icon.onMouseRightClick = function() 
			this.entity.inventory_ctr.attachItem(i + 1)
		end
		playerWindow:addComponent(icon)
		icon:setTexture("Textures/inventortbg.dds")
		itemIcons[i + 1] = icon
	end

end

function update()
end

function onDestroy()
	playerWindow = nil
	equipmentIcons = nil
	itemIcons = nil
end


