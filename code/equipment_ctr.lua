
component = "equipment_ctr"
itemData = {}


function start()
end

function update()
	local transform = this.entity.Transform
	if(transform and transform:getAbsPos().y <= 2) then
		this.entity.PhysicsComponent:setMass(0)
	end
end

function onTriggerIn(trigger, enterer)
   if(trigger == this.entity and (enterer and enterer:getName() == player_entity_name)) then 
		-- enterer is warrior
		local game = thisScene():findEntityByName("game")
		if(game.inventory_ctr) then
			local item_ctr = game.inventory_ctr
			if(item_ctr) then
				item_ctr.addItem(itemData)
				Event:trigger(game_event.ITEM_BE_PICK, { entity = this.entity })
				thisScene():removeEntity(this.entity)
			end
		end
   end
end

