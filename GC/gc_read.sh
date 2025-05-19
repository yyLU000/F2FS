MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"

df -h $MOUNT_POINT

echo "[+] Dropping caches before read test..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting sequential read test..."
fio --name=read_test \
    --filename=$FILE \
    --size=3G \
    --rw=read \
    --bs=128K \
    --ioengine=sync \
    --numjobs=1 \
    --direct=1 \
    --group_reporting

echo "[+] Dropping caches again..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting random read test..."
fio --name=read_test \
    --filename=$FILE \
    --size=1G \
    --rw=randread \
    --bs=4K \
    --ioengine=sync \
    --numjobs=2 \
    --direct=1 \
    --group_reporting

echo "[+] DONE!"
