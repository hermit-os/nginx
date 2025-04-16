# compile nginx for hermit


set libhermit env variable
```bash
export LIBHERMIT=<PATH_TO_KERNEL>/kernel/target/x86_64/release/libhermit.a
```

compile nginx with hermit compiler

```bash
#/bin/sh
docker run -v /home/debian/nginx/nginx-hermit-build:/nginx \
           -v `dirname $LIBHERMIT`:/target/ \
           -it \
           ghcr.io/hermit-os/hermit-gcc:latest /bin/bash -c "cd /nginx/; rm -fr objs/; LIBHERMIT=/target/libhermit.a bash ./configure_hermit; make -j4"
```

run nginx

```bash
qemu-system-x86_64 \
     -cpu host \
     -smp 1 \
     -m 1024M \
     -device isa-debug-exit,iobase=0xf4,iosize=0x04 \
     -display none -serial stdio \
     -netdev user,id=u1,hostfwd=tcp::80-:80,hostfwd=udp::9975-:9975,net=192.168.76.0/24,dhcpstart=192.168.76.9 -device virtio-net-pci,netdev=u1,disable-legacy=on,packed=on,mq=on \
     -kernel /home/debian/hermit/hermit-loader-x86_64  \
     -initrd /home/debian/nginx/nginx-hermit-build/objs/nginx \
     -enable-kvm 
```
