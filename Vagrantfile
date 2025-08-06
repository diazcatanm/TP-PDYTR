Vagrant.configure("2") do |config|
  2.times do |num|
    config.vm.define ("vm%01d" % (num + 1)) do |machine|
      machine.vm.box = "ubuntu/jammy64"
      machine.vm.network "private_network", ip: "192.168.56.10#{num}"
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
