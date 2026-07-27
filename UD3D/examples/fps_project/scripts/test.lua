local timer = 0

function OnStart()
   print("Lua File Started !")
end

function OnUpdate(deltaTime)
    timer = timer + deltaTime
end

function OnShutdown()
   print("Lua File Shutdown !")
end