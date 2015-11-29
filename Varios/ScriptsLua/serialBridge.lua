mode = 'telnetTCP';

function parseCommand(str)
  if(string.find(str,'ESP8266:')) then
    if(string.find(str,'BridgeMode')) then
      mode = 'bridge'
      node.output(nil, 0)
    end
    if(string.find(str,'TelnetTCPMode')) then
      mode = 'telnetTCP'
      node.output(sendToNetwork, 0)
    end
    if(string.find(str,'TelnetSerialMode')) then
      mode = 'telnetSerial'
      node.output(sendToSerial, 0)
    end
    return true
  else
    return false
  end
end

function sendToLua(str)
  node.input(str)
end
function sendToSerial(str)
  uart.write(0,str)
end


s=net.createServer(net.TCP,180) 
s:listen(180,function(c)

function sendToNetwork(str)
  if(c~=nil) then 
    c:send(str)
  end
end

function processFromSerial(str) 
  if(parseCommand(str)) then return end

  if(mode == 'telnetSerial') then
    sendToLua(str)
  end
  if(mode == 'telnetTCP') then
    if(c~=nil) then 
    
    end
  end
  if(mode == 'bridge') then
    sendToNetwork(str)
  end
end

function processFromNetwork(str)
  if(parseCommand(str)) then return end

  if(mode == 'telnetSerial') then
    
  end
  if(mode == 'telnetTCP') then
    sendToLua(str)
  end
  if(mode == 'bridge') then
    sendToSerial(str)
  end
end 
uart.on("data",processFromSerial,0)
c:on("receive",function(c,l) 
   processFromNetwork(l)
  end) 
end)

parseCommand('ESP8266:TelnetSerialMode')