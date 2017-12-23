
component = "warrior_ctr_attack"
local spawner = {}
local animAttack = -1
local animator = {}

function start()
	local cmp = this.entity.warrior_ctr_attack
	spawner = thisScene():findEntityByName("spawner")
	animator = this.entity.Animator
	if(animator) then
		animAttack = animator:getPropertyIndex("attack")
	end
end


function onInput(evt) 
   if(evt.type == FW.InputEvent.EventType.KeyPressed) then
       if(FW.Keyboard.Key.X == evt.key.code) then
	      if(animator) then
		animator:setFloatProperty(animAttack, 1)
	      end

	      local spawnercmp = spawner.spawner
		  if(spawnercmp) then
		    local mob = spawnercmp.mobList[1]
			if(mob) then
				local mobhealth = mob.mob_ctr_health
				if(mobhealth)  then mobhealth.beAttack(50) end
			end
		  end
	end
   end
end

