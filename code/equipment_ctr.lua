
component = "equipment_ctr"
itemData = {}
local onGround = false


function onTriggerIn(trigger, enterer)
   if(trigger == this.entity) then
	if(enterer and enterer:getName() == player_entity_name) then 
		-- enterer is warrior
		local game = thisScene():findEntityByName("game")
		if(game.inventory_ctr) then
			local item_ctr = game.inventory_ctr
			if(item_ctr) then
				item_ctr.addItem(itemData)
			end
		end
		Event:trigger(game_event.ITEM_BE_PICK, { entity = this.entity })
		thisScene():removeEntity(this.entity)
	elseif(enterer and enterer:getName() == "terrain") then 
		this.entity.Rigidbody:setMass(0)
	end
   end
end

