
This is hUe.

A hue dashboard that uses logistic regression to predict when to turn on/off your lights for better home automation.

Completed by:
Timal Peramune, Justine Kim, Jake Schindler, Paul Salvatore , Gurkiran Kaur Tatla 



1) Installation:

	MAC

		Install armadillo using brew:
			brew install armadillo


		Install Wt on Sierra:
			Download wt version 3.3.9 from:
				https://www.webtoolkit.eu/wt/download

			In the wt directory:
				mkdir build
				cd build
				cmake ../

				Note: the next step is very long
				make -j4		

				sudo make DESDIR=/usr/lib install


	UBUNTU (VM)

		1) EXPAND THE VM SIZE:

			cd ~/VirtualBox\ VMs/
			cd wt-vm/
			VBoxManage clonehd "wt-vm-disk1.vmdk" "wt-vm_expanded.vdi" --format vdi
			VBoxManage modifyhd "wt-vm_expanded.vdi" --resize 30000

			Then in virtual box go to: Machine > New > Expert Mode
				Name: wt-vm-expanded
				Type: Other
				Version: Other/Unknown (64-bit)
				Memory: 4096 MB

				Under hard disk:
				Use an existing virtual hard disk file, then select wt-vm_expanded.vdi (Normal 29.30GB)

			We will use wt-vm-expanded for the rest of runningYou can then start the vm as normal.

		2) IN CONSOLE OF YOUR COMPUTER, NOT YOUR VM:

			VBoxManage setextradata wt-vm_expanded VBoxInternal2/SharedFoldersEnableSymlinksCreate/wt-vm_expanded 1

		3) INSTALL THE VM GUI (as per instructions posted on owl)

		4) ADD A SHARED FOLDER TO THE VM (as per instructions on owl)

		5) DOWNLOAD JAVA (TO RUN THE EMULATOR ON THE VM)

			sudo apt-get update && sudo apt-get upgrade
			sudo apt-get install software-properties-common
			sudo add-apt-repository ppa:webupd8team/java
			sudo apt-get update
			sudo apt-get install oracle-java8-installer

		6) DOWNLOAD THE HUE EMULATOR AND PLACE INTO THE SHARED FOLDER

			You can download the emulator from:
				https://steveyo.github.io/Hue-Emulator/

			You can run the hue emulator with:
				sudo java -jar HueEmulator-v0.6.jar &

			*** ENSURE THE EMULATOR IS RUNNING ON PORT 8001 ***

		7) DOWNLOAD AND INSTALL OUR DEPENDENCY (ARMADILLO)

			You can download armadillo from:
				http://arma.sourceforge.net/download.html

			Place the unpacked directory onto your ubuntu desktop:
				mv armadillo* ~/Desktop
				cd ~/Desktop/armadillo*

			sudo apt-get install libblas-dev liblapack-dev

			In the armadillo directory:
				cmake .
				make
				sudo make install

2) Compiling

	chmod 777 build.sh

	./build.sh

3) Running

	chmod 777 run.sh

	./run.sh

4) Navigate to http://localhost:8003
