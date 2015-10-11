kvm -boot a -m 32M -fda floppy.img -net nic,vlan=0 -net user,vlan=0 -localtime \
-serial stdio -vga std
