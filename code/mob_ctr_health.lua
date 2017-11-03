component = "mob_ctr_health"


local health = 50
local spawner = {}

function start()
	spawner = thisScene():findEntityByName("spawner")
end

function update()
end

function beAttack(damage)
	health = health - damage
	if(health <= 0) then
		onDie()
	end
end

function onDie()
	Event:trigger(game_event.MOB_DIE, { entity = this.entity })
	thisScene():removeEntity(this.entity)
end
