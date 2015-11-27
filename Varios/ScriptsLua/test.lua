
print("Wake up...\n")

function led(r,g,b)
  pwm.setduty(rpin,r)
  pwm.setduty(gpin,g)
  pwm.setduty(bpin,b)
end

function setupLED(r,g,b)
  print("Setting up rgb led(devboard)")
  pwm.setup(rpin,500,512)
  pwm.setup(gpin,500,512)
  pwm.setup(bpin,500,512)
  pwm.start(rpin)
  pwm.start(gpin)
  pwm.start(bpin)
  led(0,0,0)
  tmr.delay(200000)
  led(255,0,0)
  tmr.delay(200000)
  led(0,0,0)
  tmr.delay(100000)
  led(0,255,0)
  tmr.delay(200000)
  led(0,0,0)
  tmr.delay(100000)
  led(0,0,255)
  tmr.delay(200000)
  led(0,0,0)
  tmr.delay(100000)
  led(512,512,512)
  tmr.delay(200000)
  led(0,0,0)
end  

function initWifi()
	print("Checking wifi conection...")
	print(wifi.sta.status())
	tmr.delay(1000000)
	if wifi.sta.status() == 255 then
		print("No wireless config, going to configure mode")
		setAP("ESP-telnet2323","")
	elseif wifi.sta.status() ~= 5 then
		print("Cannot connect... retry 10s")
		print(wifi.sta.status())
		tmr.delay(10000000)
		if wifi.sta.status() ~= 5 then
			print("Cannot connect... retry 15s")
			print(wifi.sta.status())
			tmr.delay(15000000)
			if wifi.sta.status() ~= 5 then
				print("Cannot connect... going to configure mode")
				print(wifi.sta.status())
				setAP("ESP-telnet2323","")
			end
		end
	end
end

function setWifiPwd(u,p)
	print("This should disconect you... see you on the other side!")
	print("(reboot if unsuccessfull)")
	tmr.delay(1000000)
	wifi.setmode(wifi.STATION)
	wifi.sta.config(u,p)
end

function setAP(ssid,pwd)
	print("Configuring AP :")
	wifi.setmode(wifi.SOFTAP)
	print(ssid)
	cfg={}
	cfg.ssid=ssid
	cfg.pwd=pwd
	wifi.ap.config(cfg)
end

function openTelnet()
	print("opening telnet on port 2323")
	s=net.createServer(net.TCP,180)
	s:listen(2323,function(c)
	   function s_output(str)
		  if(c~=nil)
			 then c:send(str)
		  end
	   end
	   node.output(s_output, 0)   -- re-direct output to function s_ouput.
	   c:on("receive",function(c,l)
		  node.input(l)           -- works like pcall(loadstring(l)) but support multiple separate line
	   end)
	   c:on("disconnection",function(c)
		  node.output(nil)        -- un-regist the redirect output function, output goes to serial
	   end)
	   print("Your wish, my command :)")
	   print("To connect to a Wifi network issue:\n setWifiPwd(\"AP\",\"Password \")")
	end)
end

function init()
	print("setup")
	rpin = 8
	gpin = 6
	bpin = 7
	setupLED(rpin,gpin,bpin)
	initWifi()
	openTelnet()

	print("Ready to Rock, ip:")
	print(wifi.sta.getip())
end

init()
