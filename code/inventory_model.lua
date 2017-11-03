component = "inventory_model"

local playerinfo = {}

playerinfo.item = { }
for i=1,MAX_INVENTORY_ITEM_COUNT do
	playerinfo.item[i] = INVALID_ITEM
end

playerinfo.equipment = {}
for i = 1,EQUIPMENT_TYPE.MAX do
	playerinfo.equipment[i] = INVALID_ITEM
end

local loaded = false

function start()
end

function getItemByPosition(itemPos)	
	return playerinfo.item[itemPos]
end

function addItem(item)				-- public
	for i,v in ipairs(playerinfo.item) do
		if(v == INVALID_ITEM) then
			playerinfo.item[i] = item
			Event:trigger(game_event.ITEM_DATA_CHANGE, { pos = i, itemdata = item })
			break
		end
	end
end

function removeItem(itemPos)				-- public
	playerinfo.item[itemPos] = INVALID_ITEM
	Event:trigger(game_event.ITEM_DATA_CHANGE, { pos = itemPos, itemdata = INVALID_ITEM })
end



function getEquipmentByPosition(equipmentPos)	
	return playerinfo.equipment[equipmentPos]
end

function attachEquipment(itemData) 
	local equipmentPos = itemData.type
	playerinfo.equipment[equipmentPos] = itemData
	Event:trigger(game_event.EQUIPMENT_DATA_CHANGE, { pos = equipmentPos, itemdata = itemData })
end

function detachEquipment(equipmentPos) 
	playerinfo.equipment[equipmentPos] = INVALID_ITEM
	Event:trigger(game_event.EQUIPMENT_DATA_CHANGE, { pos = equipmentPos, itemdata = INVALID_ITEM })
end








local function loadGameData()
  for _, item in ipairs(game_db.item_db) do
	   addItem(item)
   end
end

function update()
	if(loaded == false) then
		loadGameData()
		loaded = true
	end
end

