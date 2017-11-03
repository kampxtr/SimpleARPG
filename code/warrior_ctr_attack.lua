
component = "warrior_ctr_attack"
local spawner = {}

function start()
	local cmp = this.entity.warrior_ctr_attack
	spawner = thisScene():findEntityByName("spawner")
	this.entity.Animator:setAnimation("./Animations/Warrior_Idle.anim")
end

function update()
end

function onInput(evt) 
   if(evt.type == FW.InputEvent.EventType.KeyPressed) then
       if(FW.Keyboard.Key.X == evt.key.code) then
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

