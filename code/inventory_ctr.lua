

component = "inventory_ctr"

local equipments = {}

function start()
	
end

function update()
end

function onDestroy()
end

function addItem(itemData)
	if(this.entity.inventory_model) then
		this.entity.inventory_model.addItem(itemData)
	end
end

function attachItem(itemPos) 
	local itemData = this.entity.inventory_model.getItemByPosition(itemPos)
	if(itemData and itemData ~= INVALID_ITEM) then
		this.entity.inventory_model.removeItem(itemPos)
		this.entity.inventory_model.attachEquipment(itemData)
		local res = loadXml(itemData.prefab)
		local equipment = thisScene():createEntityFromResource(res)
		equipments[itemData.type] = equipment
		local hero = thisScene():findEntityByName("warrior")
		attachEquipmentToHero(hero, itemData.type, equipment)
	end
end

function detachEquipment(equipmentPos) 
	local itemData = this.entity.inventory_model.getEquipmentByPosition(equipmentPos)
	if(itemData ~= INVALID_ITEM) then
		this.entity.inventory_model.addItem(itemData)
		local hero = thisScene():findEntityByName("warrior")
		local equipment = equipments[itemData.type]
		if(equipment) then
			detachEquipmentToHero(hero, itemData.type, equipment)
		end
	end
	this.entity.inventory_model.detachEquipment(equipmentPos)
end
