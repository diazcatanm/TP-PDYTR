
Vagrant.configure("2") do |config|
  # Definición de las máquinas virtuales
  # vm1: Servidor (IP estática)
  # vm2: Cliente (DHCP o IP dinámica)
  
  {
    "vm1" => "192.168.1.45",
    "vm2" => nil
  }.each do |name, ip|
    config.vm.define name do |machine|
      machine.vm.box = "ubuntu/jammy64"
      machine.vm.hostname = name

      # Configuración de Red
      # Bridge >> para que las VMs se vean entre sí y con el host
      if name == "vm1"
        machine.vm.network "public_network", bridge: "wlp0s20f3", ip: ip
      else
        machine.vm.network "public_network", bridge: "wlp0s20f3", use_dhcp_assigned_default_route: true
      end

      # --- CARPETA COMPARTIDA ---
      machine.vm.synced_folder ".", "/vagrant_shared"

      # Recursos de Hardware
      machine.vm.provider "virtualbox" do |vb|
        vb.memory = "512"
        vb.cpus = 1
        vb.name = "Sockets_#{name}"
      end

      # Aprovisionamiento: Instalamos herramientas necesarias
      machine.vm.provision "shell", inline: <<-SHELL
        apt-get update -y
        apt-get install -y build-essential net-tools psmisc
        # psmisc instala 'fuser', necesario para limpiar los puertos trabados
      SHELL
    end
  end
end
