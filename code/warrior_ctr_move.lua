

component = "warrior_ctr_move"

function start() end

function update() end

function onInput(evt) 
   if(evt.type == FW.InputEvent.EventType.KeyPressed) then
       local moveamt = Vec3f.new()
	   local walkspeed = 10
       if(FW.Keyboard.Key.Left == evt.key.code) then
		   moveamt.x = -walkspeed
       elseif(FW.Keyboard.Key.Right == evt.key.code) then
		   moveamt.x = walkspeed
       elseif(FW.Keyboard.Key.Up == evt.key.code) then
		   moveamt.z = walkspeed
       elseif(FW.Keyboard.Key.Down == evt.key.code) then
		   moveamt.z = -walkspeed
	   end
	   local transform = this.entity.Transform
	   local body = this.entity.PhysicsComponent
	   if(body and transform) then
		   body:setPos(transform:getAbsPos() + moveamt)
	   end
   end
end
