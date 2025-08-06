#base_network = "192.168.1."
#base_host = 100

Vagrant.configure("2") do |config|
  2.times do |num|
    config.vm.define ("vm%01d" % (num + 1)) do |machine|
      machine.vm.box = "ubuntu/jammy64"
     
      if (num + 1 == 2) then
      	machine.vm.network "public_network", bridge: "wlp0s20f3", use_dhcp_assigned_default_route: true
      else
      	machine.vm.network "public_network", bridge: "wlp0s20f3", type:"static", ip:"192.168.1.45"
      end if
      #, ip: "#{base_network}#{base_host + num}"
      machine.vm.hostname = "vm#{num + 1}"

      machine.vm.synced_folder "./shared", "/vagrant_shared"

      machine.vm.provider "virtualbox" do |vb|
        vb.memory = "512"
      end

      machine.vm.provision "shell", inline: <<-SHELL
        apt-get update
        apt-get upgrade
        apt-get install -y build-essential net-tools zip joe default-jdk
      SHELL
    end
  end
end
