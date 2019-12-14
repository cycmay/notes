# Ubuntu 安装CUDA开发环境

## Ubuntu 系统

### 1 download

下载地址：[Ubuntu官方镜像 18.04 LTS](https://ubuntu.com/download/desktop)

### 2 refus

下载地址：[官方](https://rufus.ie/)

使用方法：

仔细看官方教程，选择镜像文件后默认即可。

设置200g的ubuntu磁盘空间，空白即可

### 3 重启 修改boot

CUDA安装需要修改secure boot 设置为disable

ubuntu系统安装需要设置boot启动顺序，自行调整

### 4 安装ubuntu

按照安装提示，自行选择，直到安装磁盘控制，选择something else

500m boot

TODO: 查看教程写仔细

## Ubuntu 更新

### 更新源

在Software & Updates，将源更新为阿里云的源

mirror.aliyun.edu.cn

手动更新apt

`$ sudo apt update`

`$ sudo apt upgrade`

### Sougou Pinyin

`$ sudo apt-get install fcitx-bin      #安装fcitx-bin
`$ sudo apt-get update --fix-missing   #修复fcitx-bin安装失败的情况
`$ sudo apt-get install fcitx-bin      #重新安装fcitx-bin
`$ sudo apt-get install fcitx-table    #安装fcitx-table`

然后去[搜狗输入法Linux官网](https://pinyin.sogou.com/linux/)下载64bit的程序，我的是：sogoupinyin_2.2.0.0108_amd64.deb

`$ sudo dpkg -i sogoupinyin*.deb       #安装搜狗拼音`
`$ sudo apt-get install -f             #修复搜狗拼音安装的错误`
`$ sudo dpkg -i sogoupinyin*.deb       #重新安装搜狗拼音`

重启
点击Show Applications（就是屏幕左下角那九个点），找到Fcitx Configure，点击进入，加入sougou

### WPS

进入[WPS_Linux官网](https://www.wps.cn/product/wpslinux/#)，下载deb文件
安装依赖libpng12-0，[下载该文件](http://kr.archive.ubuntu.com/ubuntu/pool/main/libp/libpng/libpng12-0_1.2.54-1ubuntu1_amd64.deb)

`$ sudo dpkg -i libpng12-0*.deb`

安装wps

`$ sudo dpkg -i wps*.deb`

若出现错误，或者没有安装成功，使用如下命令修复

`$ sudo apt-get install -f`

至此，wps已经安装成功。但是由于Linux版权原因，WPS缺少字体，故我们要安装WPS所需要的字体。首先下载WPS字体，然后解压。

```
sudo mkdir /usr/share/fonts/WPS-Fonts       #新建wps字体存储文件夹
cd ~/Downloads     #进入下载好的字体目录
sudo apt-get install unzip  #安装zip解压软件
sudo unzip wps_symbol_fonts.zip -d /usr/share/fonts/WPS-Fonts/  #解压字体到指定文件夹
sudo mkfontscale    #生成字体索引
sudo mkfontdir      #生成字体索引
sudo fc-cache       #更新字体缓存
```

### 视频和音频

安装解码器：

`$ sudo apt-get install ubuntu-restricted-extras`

安装VLC视频播放器

`$ sudo apt-get install vlc browser-plugin-vlc`

另一款比较好的播放器SMPlayer也不错

``` sh
sudo apt-add-repository ppa:rvm/smplayer
sudo apt-get update
sudo apt-get install smplayer smplayer-skins smplayer-themes 
```

安装FFmpeg

``` sh
sudo add-apt-repository ppa:djcj/hybrid
sudo apt-get update
sudo apt-get install ffmpeg
```

安装网易云音乐

进入网易云音乐Linux版，[下载Ubuntu16.04](https://music.163.com/#/download)（64位）。

`$ sudo dpkg -i netease-cloud-music*.deb`

## Ubuntu美化

看博客即可

安装一系列软件

TODO: 美化篇

## 系统显卡环境

### 官方教程

地址：[CUDA toolkit](https://docs.nvidia.com/cuda/index.html)

### 下载驱动

 删掉以往的驱动. 注意, 就算你啥都没装, 这步也是无害的. 

```javascript
sudo apt-get remove --purge nvidia*
```

 更新并安装一些需要的库, 先装这么多, 之后装CUDA还有一波. 

```javascript
sudo apt-get update
sudo apt-get install dkms build-essential linux-headers-generic
```

------

### 禁用nouveau

 打开blacklist.conf, 在最后加入禁用nouveau的设置: 

```javascript
sudo vim /etc/modprobe.d/blacklist.conf
blacklist nouveau
blacklist lbm-nouveau
options nouveau modeset=0
alias nouveau off
alias lbm-nouveau off
```

禁用nouveau

禁用nouveau内核模块 

```javascript
echo options nouveau modeset=0
sudo update-initramfs -u
```

### 安装驱动

```javascript
sudo chmod u+x NVIDIA-Linux-x86_64-390.87.run 
sudo ./NVIDIA-Linux-x86_64-390.87.run –no-opengl-files
```

用nvidia-smi指令测试

## 安装CUDA10.0

```shell
sudo apt-get install freeglut3-dev libx11-dev libxmu-dev libxi-dev libgl1-mesa-glx libglu1-mesa libglu1-mesa-dev
```

 到[官网](https://developer.nvidia.com/cuda-toolkit-archive)下载要的CUDA版本, 我这里是[8.0第二个版本](https://developer.nvidia.com/cuda-80-ga2-download-archive), 下载runfile(local)版本, 如下图所示: 

md5检测一下, 不合格要重新下载. 下图是我的检测结果: 

```javascript
md5sum cuda_8.0.61_375.26_linux.run
```



### md5检测

 安装时候依旧要加**–no-opengl-files**参数, 然后就是先一直空格, 之后一路默认就好. 

```javascript
sudo sh cuda_8.0.61_375.26_linux.run –no-opengl-files
```

然后会看到三个installed. 

  运行如下命令, 如果显示如图3个文件夹, 恭喜你. 

```javascript
ls /dev/nvidia*
```

 如果少了或者都找不到, 还有一些操作要做. 用vim创建一个xxx.sh(名字随意), 输入以下内容: 

```javascript
#!/bin/bash

/sbin/modprobe nvidia

if [ "$?" -eq 0 ]; then
  # Count the number of NVIDIA controllers found.
  NVDEVS=`lspci | grep -i NVIDIA`
  N3D=`echo "$NVDEVS" | grep "3D controller" | wc -l`
  NVGA=`echo "$NVDEVS" | grep "VGA compatible controller" | wc -l`

  N=`expr $N3D + $NVGA - 1`
  for i in `seq 0 $N`; do
    mknod -m 666 /dev/nvidia$i c 195 $i
  done

  mknod -m 666 /dev/nvidiactl c 195 255

else
  exit 1
fi

/sbin/modprobe nvidia-uvm

if [ "$?" -eq 0 ]; then
  # Find out the major device number used by the nvidia-uvm driver
  D=`grep nvidia-uvm /proc/devices | awk '{print $1}'`

  mknod -m 666 /dev/nvidia-uvm c $D 0
else
  exit 1
fi
```

运行后就会看到3个文件夹. 

```javascript
sudo chmod +x xxx.sh
sudo ./xxx.sh
ls /dev/nvidia*
/dev/nvidia0  /dev/nvidiactl  /dev/nvidia-uvm
```

然后写入rc.local中, 如图所示: 

```javascript
sudo vim /etc/rc.local
```

rc.local

 打开环境配置文件 

```javascript
sudo vim /etc/profile
```

 最后写入: 

```javascript
export PATH=/usr/local/cuda-8.0/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64:$LD_LIBRARY_PATH
```

保存退出, 并其生效. 

```javascript
source /etc/profile
```

然后就能看到了. 

 运行一些检测命令, 如果和我显示的类似, 恭喜你, 环境配置完成. 

```javascript
cat /proc/driver/nvidia/version
nvcc -V
```