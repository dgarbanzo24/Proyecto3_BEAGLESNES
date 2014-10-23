#!/bin/sh                                                                       
                                                                                
mount -o remount,rw /dev/mmcblk0p2 /                                            
mount -o rw /dev/mmcblk0p1 /boot/uboot/                                         
#mount -t devtmpfs,tmpfs -o mode=0755 /dev                                      
#mount -t tmpfs tmpfs /run                                                      
#/sbin/udevd --daemon                                                           
#mount -t sysfs sysfs /sys                                                      
#mount -t proc proc /proc                                                       
#udevadm trigger                                                                
#/usr/sbin/alsactl restore                                                      
/home/root/beaglesnes/service.sh                                                
/bin/sh           
