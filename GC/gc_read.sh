echo "[+] Starting sequential read test..."
fio --name=read_test \
    --filename=$FILE \
    --size=1G \
    --rw=read \
    --bs=4K \
    --ioengine=sync \
    --numjobs=1 \
    --runtime=120 \
    --direct=1 \
    --time_based \
    --group_reporting

echo "[+] Starting random read test..."
fio --name=read_test \
    --filename=$FILE \
    --size=1G \
    --rw=randread \
    --bs=4K \
    --ioengine=sync \
    --numjobs=4 \
    --runtime=120 \
    --direct=1 \
    --time_based \
    --group_reporting
