# Docker

## 背景

**Docker 属于 Linux 容器的一种封装，提供简单易用的容器使用接口。**它是目前最流行的 Linux 容器解决方案。

Docker 将应用程序与该程序的依赖，打包在一个文件里面。运行这个文件，就会生成一个虚拟容器。程序在这个虚拟容器里运行，就好像在真实的物理机上运行一样。有了 Docker，就不用担心环境问题。

总体来说，Docker 的接口相当简单，用户可以方便地创建和使用容器，把自己的应用放入容器。容器还可以进行版本管理、复制、分享、修改，就像管理普通的代码一样。

Docker 的主要用途，目前有三大类。

**（1）提供一次性的环境。**比如，本地测试他人的软件、持续集成的时候提供单元测试和构建的环境。

**（2）提供弹性的云服务。**因为 Docker 容器可以随开随关，很适合动态扩容和缩容。

**（3）组建微服务架构。**通过多个容器，一台机器可以跑多个服务，因此在本机就可以模拟出微服务架构。



## 项目开发重构

为了安全起见，在生产环境我们将 Django/project/setting.py 的 DEBUG 选项改为 False，但开发环境要改为 True，这样就造成了开发与生产需要来回更改配置的局面。此外，django 的 SECRET_KEY 是很私密的配置，django 的很多安全机制都依赖它，如果像现在这样直接写在配置文件中，不小心公开了源代码，SECRET_KEY 就会直接泄露，一个比较好的方法就是将这个值写入环境变量，通过从环境变量取这个值。

解决以上问题的一个方案就是拆分 settings.py 文件，不同环境对应不同的 settings 文件，django 在启动时会从环境变量中读取 `DJANGO_SETTINGS_MODULE` 的值，以这个值指定的文件作为应用的最终配置。

### settings.py 拆分

首先在 project (project/project, 项目配置文件目录)目录下新建一个 Python 包，名为 settings，然后创建一个 common.py 用于存放通用配置，local.py 存放开发环境的配置，production.py 存放线上环境的配置：

```
project\
    settings\
        __init__.py
        common.py
        local.py
        production.py
    settings.py	# 删掉Django自动生成的配置文件settings.py
```

将 settings.py 文件中的内容全部复制到 common.py 里，并将 `SECRET_KEY`、`DEBUG`、`ALLOWED_HOSTS` 这些配置移到 local.py 和 production.py 中（common.py 中这些项可以删除）.

开发环境的配置 local.py 内容如下：

```python
from .common import *
 
SECRET_KEY = 'development-secret-key'
DEBUG = True
ALLOWED_HOSTS = ['*']
```

线上环境的配置：

```python
from .common import *
 
SECRET_KEY = os.environ['DJANGO_SECRET_KEY']
DEBUG = False
ALLOWED_HOSTS = ['127.0.0.1'， '#生产环境域名/ip地址']
```

线上环境和开发环境不同的是，为了安全，DEBUG 模式被关闭，SECRET_KEY 从操作系统环境变量获取，ALLOWED_HOSTS 设置了允许的 HTTP HOSTS.

以上操作完成后，**删除 settings.py**.

### Django 识别分离后的配置

在使用 python manage.py 执行命令时，django 可以接收一个 --settings-module 的参数，用于指定执行命令时，项目使用的配置文件，如果参数未显示指定，django 会从环境变量 DJANGO_SETTINGS_MODULE 里获取。manage.py 的源码：

```python
# manage.py
if __name__ == "__main__":
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "project.settings")
    try:
        from django.core.management import execute_from_command_line
    except ImportError:
```

在 main 中，第一行的 setdefault 设置了环境变量 `DJANGO_SETTINGS_MODULE` 的值。这句代码的作用是，如果当前环境中 `DJANGO_SETTINGS_MODULE` 的值没有被设置，就将其设置为 `project.settings`。当用户使用 `python manage.py` 执行命令时，Django 默认为用户使用了 settings.py 这个文件所有的配置项。

同时用户可以通过设置环境变量，来指定 Django 使用的配置文件。`python manage.py` 通常在开发环境下执行，因此将这里的 `DJANGO_SETTINGS_MODULE` 的值改为 `project.settings.local`，这样运行开发服务器时 Django 会加载 project/settings/local.py 这个配置文件。另外在 `project/project/wsgi.py` 文件中，这个文件中有一个 application，是在线上环境时 Gunicorn 加载运行的，将这里面的 `DJANGO_SETTINGS_MODULE` 改为 `blogproject.settings.production`

```python
# project/wsgi.py
import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "project.settings.production")

application = get_wsgi_application()
```

这样，在使用 manage.py 执行命令时，加载的是 local.py 的设置，而使用 gunicorn 运行项目时，使用的是 production.py 的设置。

### 修改 BASE_DIR 配置项

需要注意的一点，在存放通用配置的 common.py 文件，里面有一个配置项为：

```python
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
```

这个 `BASE_DIR` 指向项目根目录，其获取方式为根据所在的配置文件向上回溯，找到项目根目录。因为此前的目录结构为 project/project/settings.py，因此向上回溯 2 层就到达项目根目录。而现在目录结构为project/project/settings/common.py，需向上回溯 3 层才到达项目根目录，因此需将 `BASE_DIR` 进行一个简单修改，修改如下：

```python
BASE_DIR = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
```

即再在外面包一层 os.path.dirname，再向上回退一层，到达项目根目录。

### 设置 Supervisor 环境变量

TODO: 更新无supervisor的系统

此外，由于线上环境配置中的 secret_key 从环境变量获取，因此我们改一下 supervisor 的配置，将环境变量导入，打开 supervisor 的配置文件 ~/etc/supervisor/conf.d/project.ini，添加环境变量的配置语句：

```
environment=DJANGO_SECRET_KEY=development-secret-key
```

因为此前可能将代码传过公开的代码仓库，所以最好把线上使用的 SECRET_KEY换一下。这个网站可以自动生成 SECRET_KEY：[Django Secret Key Generator](https://www.miniwebtool.com/django-secret-key-generator/)。

保存配置，然后要执行 update 命令更新配置。

```bash
$ supervisorctl -c ~/etc/supervisord.conf update
```

## Docker 编排项目

之前一系列繁琐的部署步骤让我们感到痛苦。这些痛苦包括：

- 要去服务器上执行 n 条命令
- 本地环境和服务器环境不一致，明明本地运行没问题，一部署服务器上就挂挂，死活启动不起来
- 如果上面的情况发生了，又要去服务器上执行 n 条命令以解决问题
- 本地更新了代码，部署上线后，上述历史又重演一遍，想死的心都有了

那么我们有没有办法，让本地开发环境和线上环境保持一致？这样我们在部署上线前，就可以在本地进行验证，只要验证没问题，我们就有 99% 的把握保证部署上线后也没有问题（1%保留给程序玄学）。

这个办法就是使用 Docker。

### 编排镜像目录

类似于分离 settings.py 文件为 local.py 和 production.py，我们首先建立如下的目录结构，分别用于存放开发环境的镜像和线上环境的镜像：

```
project\
      blog\
      ...
      compose\		#编排docker目录
            local\
            production\
                  django\
                  nginx\
    ...
```

local 目录下存放本地开发环境的 Docker 镜像文件，production 下的 Django 文件夹存放基于本项目编排的镜像，由于线上环境还要用到 Nginx，nginx 目录下存放 Nginx 的镜像。

### 生产环境镜像

#### 项目镜像

首先在 production\django 目录下编排项目生产环境的镜像文件，镜像文件以 Dockerfile 命名：

```dockerfile
FROM python:3.7-alpine

ENV PYTHONUNBUFFERED 1

# 设置清华源apk
RUN sed -i 's/dl-cdn.alpinelinux.org/mirror.tuna.tsinghua.edu.cn/g' /etc/apk/repositories

RUN apk update \
  # Pillow dependencies
 && apk add g++ jpeg-dev zlib-dev freetype-dev lcms2-dev openjpeg-dev tiff-dev tk-dev tcl-dev

WORKDIR /app

RUN pip install pipenv -i https://pypi.douban.com/simple

COPY Pipfile /app/Pipfile
COPY Pipfile.lock /app/Pipfile.lock
RUN pipenv install --system --deploy --ignore-pipfile

COPY . /app

COPY ./compose/production/django/start.sh /start.sh

RUN chmod +x /start.sh
```

首先我们在镜像文件开头使用 `FROM python:3.6-alpine` 声明此镜像基于 python:3.6-alpine 基础镜像构建。alpine 是一个 Linux 系统发行版，主打小巧、轻量、安全。我们程序运行需要 Python 环境，因此使用这个小巧但包含完整 Python 环境的基础镜像来构建我们的应用镜像。

`ENV PYTHONUNBUFFERED 1` 设置环境变量 PYTHONUNBUFFERED=1

接下来的一条 RUN 命令安装图像处理包 Pilliow 的依赖，因为如果使用 Django 处理图片时，会使用到 Pillow 这个Python 库。

接着使用 WORKDIR /app 设置工作目录，以后在基于此镜像启动的 Docker 容器中执行的命令，都会以这个目录为当前工作目录。注意这个/app是存在于docker容器的隔离环境中。

然后我们使用命令 `RUN pip install pipenv` 安装 pipenv，-i 参数指定 pypi 源，国内一般指定为豆瓣源，这样下载 pipenv 安装包时更快，国外网络可以省略 -i 参数，使用官方的 pypi 源即可。

然后我们将项目依赖文件 Pipfile 和 Pipfile.lock copy 到容器里，运行 pipenv install 安装依赖。**指定 --system 参数后 pipenv 不会创建虚拟环境，而是将依赖安装到容器的 Python 环境里。因为容器本身就是个虚拟环境了，所以没必要再创建虚拟环境**。

接着将这个项目的文件 copy 到容器的 /app 目录下，当然有些文件对于程序运行是不必要的，所以我们会设置一个 dockerignore 文件，里面指定的文件不会被 copy 到容器里。

然后我们还将 start.sh 文件复制到容器的 / 目录下，并赋予了可执行权限。

#### Nginx 镜像

线上环境使用 Nginx，同样来编排 Nginx 的镜像，这个镜像文件放到 compose\production\nginx 目录下：

```dockerfile
FROM nginx:1.17.1
RUN apt-get update && apt-get install -y --allow-unauthenticated certbot python-certbot-nginx

RUN rm /etc/nginx/conf.d/default.conf
COPY ./compose/production/nginx/project.conf /etc/nginx/conf.d/project.conf
```

最后就是把应用的 nginx 配置复制到容器中 nginx 的 conf.d 目录下。里面的内容和直接在系统中配置 nginx 是一样的。

```nginx
# /project/compose/production/nginx/porject.conf
upstream project  {
    server project:8000;		# 类似于负载均衡 将代理 proxy_pass 转发给container project
}
 
server {
    listen 80;	# 监听80端口
    server_name  project.ip or domain;	#服务器名，多个用空格隔开 

    location /static {
        alias /app/static;	# 访问到/static/时 Nginx负责将其映射到 /app/static/ 目录
    }
 
    location / {
        proxy_pass http://project;	# 访问到Nginx的根目录/时，转发到project服务器 见解释1
        proxy_pass_header       Authorization;
        proxy_pass_header       WWW-Authenticate;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```

相比之前直接在宿主机配置 Nginx，这里使用了 Nginx 的 upstream 模块，实际上就是做一个请求转发。Nginx 将所有请求转发给上游 project 模块处理，而 project 这个模块的服务实际就是运行 django 应用的容器 project（接下来会运行这个容器）。

解释1：

Change your proxy_pass from

```
proxy_pass         http://0.0.0.0:8000;
```

to

```
proxy_pass         http://web:8000;
```

Your nginx needs to forward to request the web container

**Edit-1: Explanation**

`0.0.0.0` is a special IP address which is used to refer to any available interface on the machine. So if your machine has a loopback (lo), ethernet (eth0), Wifi (wlan0) with respective IP as `127.0.0.1`, `192.168.0.100`, `10.0.0.100`.

So now while listening for incoming connection you can choose any of the above IP

```
gunicorn wsgi:application --workers 2 --bind 10.0.0.100:8000
```

This will only be reachable from your Wifi network. But other machines on Lan network can't visit it. So if you want your app to listen on any available network on the machine you use a special IP `0.0.0.0`. This means bind on all network interfaces

```
gunicorn wsgi:application --workers 2 --bind 0.0.0.0:8000
```

Now when you access the app using `http://0.0.0.0` it is equivalent to using `127.0.0.1`. So your `proxy_pass http://0.0.0.0:8000;` is equivalent to `proxy_pass http://127.0.0.1:8000;`

So when you run that in nginx container, it passes on the request on port 8000 of the same container and there is nothing running on 8000 in your nginx container. So you need to send that request to the your gunicorn container. This is reachable using the service name `web` in `docker-compose`.

See the below article for more details https://docs.docker.com/compose/networking/

#### Docker启动项目

start.sh 中就是启动 Gunicorn 服务的命令：

```sh
#!/bin/sh

python manage.py migrate
python manage.py collectstatic --noinput
# media目录保存项目生成的图片
dir="./static/media"
if [ ! -d "$dir" ];then
mkdir -p $dir
echo "created ./static/media !"
else
echo "./static/media already exsited!"
fi
gunicorn vehicleBrand.wsgi:application -w 4 -k gthread -b 0.0.0.0:8000 --chdir=/app
```

我们会让容器启动时去执行此命令，这样就启动了我们的 Django 应用。--chdir=/app 表明以 /app 为根目录，这样才能找到 project.wsgi:application。

在项目根目录下建立 .dockerignore 文件，指定**不** copy 到容器的文件：

```dockerignore
.*	
_credentials.py
fabfile.py
*.sqlite3
```



镜像编排完毕，接下来就可以通过镜像构建容器并运行容器了。但是，我们有两个镜像，一个是 django 应用的，一个是 Nginx 的，这意味着我们需要构建 2 次容器，并且启动容器 2 次，这会比较麻烦。有没有办法一次构建，一条命令运行呢？答案就是使用 docker-compose。

docker-compose 将各个容器的镜像，以及构建和运行容器镜像时的参数等编写在一个 ymal 文件里。这样我们只需要一条 build 命令就可以构建多个容器，使用一条命令 up 就可以启动多个容器。

我们在项目根目录建一个 production.yml 文件来编排 django 容器和 nginx 容器。

```yaml
# vehiclebrand 是本项目名
version: '3'

volumes:
  static:
  database:

services:
  vehiclebrand:
    build:
      context: .
      dockerfile: compose/production/django/Dockerfile
    image: vehiclebrand
    container_name: vehiclebrand
    working_dir: /app
    volumes:
      - database:/app/database
      - static:/app/static
    env_file:
      - .envs/.production
    ports:
      - "8000:8000"
    command: /start.sh

  nginx:
    build:
      context: .
      dockerfile: compose/production/nginx/Dockerfile
    image: vehiclebrand_nginx
    container_name: vehiclebrand_nginx
    volumes:
      - static:/app/static
    ports:
      - "80:80"
      - "443:443"
```

`version: '3'` 声明 docker-compose 为第三代版本的语法

```yaml
volumes:
  static:
  database:
```

volumes 声明了 2 个命名数据卷，分别为 static 和 database。由于 docker 容器是一个隔离环境，一旦容器被删除，容器内的文件就会一并删除。一段时间后，容器中的数据库就会产生数据，后来我们更新了代码或者修改了容器的镜像，这个时候就要删除旧容器，然后重新构建新的容器并运行，那么旧容器中的数据库就会连同容器一并删除。

所以我们使用 docker 的数据卷来管理需要持久存储的数据，只要数据被 docker 的数据卷管理起来了，那么新的容器启动时，就可以从数据卷取数据，从而恢复被删除容器里的数据。

我们有 2 个数据需要被数据卷管理，一个是数据库文件，一个是应用的静态文件。数据库文件容易理解，那么为什么静态文件也要数据卷管理呢？启动新的容器后使用 python manage.py collectstatic 命令重新收集不就好了？

答案是不行，数据卷不仅有持久保存数据的功能，还有跨容器共享文件的功能。要知道，容器不仅和宿主机隔离，而且容器之间也是互相隔离的。Nginx 运行于独立容器，那么它处理的静态文件从哪里来呢？应用的静态文件存放于应用容器，Nginx 容器是访问不到的，所以这些文件也通过数据卷管理，nginx 容器从数据卷中取静态文件映射到自己的容器内部。

接下来定义了 2 个 services，一个是应用服务 project项目，一个是 nginx 服务。

```yaml
build:
  context: .
  dockerfile: compose/production/django/Dockerfile
```

告诉 docker-compose，构建容器是基于当前目录（yml 文件所在的目录），且使用的镜像是 dockerfile 指定路径下的镜像文件。

image 和 container_name 分别给构建的镜像和容器取个名字。

working_dir 指定工作目录。

```yaml
volumes:
  - database:/app/database
  - static:/app/static
```

建立数据卷和容器中文件的映射关系。

同时这里要注意，数据卷只能映射文件夹而不能映射单一的文件，所以对我们应用的数据库来说，db.sqlite3 文件我们把它挪到了 database 目录下。因此我们要改一下 django 的配置文件中数据库的配置，让它正确地将数据库文件生成在项目根目录下的 database 文件夹下：

```python
# /project/settings/common.py
DATABASES = {
    'default': {
         'ENGINE': 'django.db.backends.sqlite3',
         'NAME': os.path.join(BASE_DIR, 'database', 'db.sqlite3'),
    }
}
env_file:
  - .envs/.production
```

容器启动时读取 .envs/.production文件中的内容，将其注入环境变量。.envs 是生产环境服务器下创建。

我们创建一下这个文件，把 secret_key 写进去。

```
DJANGO_SECRET_KEY=2pe8eih8oah2_2z1=7f84bzme7^bwuto7y&f(#@rgd9ux9mp
```

**注意将这些包含敏感信息的文件加入版本控制工具的忽略列表里，防止一不小心推送到公开仓库供大众观光。**

```yaml
ports:
  - "8000:8000"
```

暴露容器内的 8000 端口并且和宿主机的 8000 端口绑定，于是我们就可以通过宿主机的 8000 端口访问容器。

command: /start.sh 容器启动时将执行 start.sh，从而启动 django应用。

nginx 服务容器也类似，只是注意它从数据卷 static 中取静态文件并映射到 nginx 容器内的 /apps/project/static，所以我们在 nginx 的配置中：

```nginx
location /static {
    alias /app/static;
}
```

这样可以正确代理静态文件。

在**本地**执行一下下面的两条命令来构建容器和启动容器。

```
docker-compose -f production.yml build
docker-compose -f production.yml up
```

此时我们可以通过域名来访问容器内的应用，当然，由于 Nginx 在本地环境的容器内运行，需要修改一下 本地 hosts 文件，让域名解析为本地 ip 即可。

如果本地访问没有问题了，那么就可以直接在服务器上执行上面两条命令以同样的方式启动容器，django 应用就顺利地在服务上部署了。

### 开发环境镜像

既然线上环境都使用 Docker 了，不妨开发环境也一并使用 Docker 进行开发。开发环境的镜像和 docker-compose 文件比线上环境简单一点，因为不用使用 nginx。

#### 项目镜像

开发环境的镜像文件，放到 compose\local 下：

```yaml
FROM python:3.7-alpine

ENV PYTHONUNBUFFERED 1

RUN sed -i 's/dl-cdn.alpinelinux.org/mirror.tuna.tsinghua.edu.cn/g' /etc/apk/repositories

RUN apk update \
  # Pillow dependencies
 && apk add g++ jpeg-dev zlib-dev freetype-dev lcms2-dev openjpeg-dev tiff-dev tk-dev tcl-dev

WORKDIR /app

RUN pip install pipenv -i https://pypi.douban.com/simple

COPY Pipfile /app/Pipfile
COPY Pipfile.lock /app/Pipfile.lock
RUN pipenv install --system --deploy --ignore-pipfile

COPY ./compose/local/django/start.sh /start.sh

RUN chmod +x /start.sh
```

#### Docker启动项目

要注意和线上环境不同的是，我们没有把整个代码 copy 到容器里。线上环境代码一般比较稳定，而对于开发环境，由于需要频繁修改和调试代码，如果我们把代码 copy 到容器，那么容器外做的代码修改，容器内部是无法感知的，这样容器内运行的应用就没法同步我们的修改了。所以我们会把代码通过 Docker 的数据卷来管理。

start.sh 不再启动 gunicorn，而是使用 runserver 启动开发服务器。

```bash
#!/bin/sh
dir="./static/media"
if [ ! -d "$dir" ];then
mkdir -p $dir
echo "created ./static/media !"
else
echo "./static/media already exsited!"
fi

python manage.py migrate
python manage.py runserver 0.0.0.0:8000
```

然后创建一个 docker-compose 文件 local.yml（和 production.yml 同级），用于管理开发容器。

```yaml
version: '3'

volumes:
  database_local:

services:
  vehiclebrand_local:
    build:
      context: .
      dockerfile: ./compose/local/django/Dockerfile
    image: vehiclebrand_local
    container_name: vehiclebrand_local
    working_dir: /app
    volumes:
      - database_local:/app/database
      - .:/app
    ports:
      - "8000:8000"
    command: /start.sh
```

注意我们将整个项目根目录下的文件挂载到了 /app 目录下，这样就能容器内就能实时反映代码的修改了。

## 服务器环境

### 创建一个超级用户

顺利连接到远程服务器了，如果是一台全新服务器的话，通常我们是以 root 用户登录的。在 root 下部署代码不够安全，最好是建一个新用户（如果你已经以非 root 用户登录的话可以跳过这一步）。下面的一些列命令将创建一个拥有超级权限的新用户（把 bicycle 替换成你自己想要的用户名，我这里取我的名字拼音 bicycle）：

```shell
# 在 root 用户下运行这条命令创建一个新用户，bicycle 是用户名
# 因为我叫杨学光，所以我取的用户名是 bicycle
# 选择一个你喜欢的用户名，不一定非得和我的相同
root@server:~# adduser bicycle

# 为新用户设置密码
# 注意在输密码的时候不会有字符显示，不要以为键盘坏了，正常输入即可
root@server:~# passwd bicycle

# 把新创建的用户加入超级权限组
root@server:~# usermod -aG wheel bicycle

# 切换到创建的新用户
root@server:~# su - bicycle

# 切换成功，@符号前面已经是新用户名而不是 root 了
bicycle@server:$
```

新用户创建并切换成功了。如果是新服务器的话，最好先更新一下系统，避免因为版本太旧而给后面安装软件带来麻烦。运行下面的两条命令：

```sh
bicycle@server:$ sudo yum update
bicycle@server:$ sudo yum upgrade
```

### 更新 SQLite3

为了方便，我们博客使用了 SQLite3 数据库，django 2.2 要求 SQLite3 数据库版本在 3.8.3 以上，而 CentOS 7 系统自带版本低于 django 2.2 所要求的最低版本，所以首先来更新 SQLite3 的版本。

> **注意**
>
> 有可能你使用的服务器系统发行版 SQLite3 已经高于 3.8.3，这一步就可以跳过。如何查看 SQLite3 的版本呢？请执行 sqlite3 --version

首先登陆到 [sqlite 的官方下载地址](https://sqlite.org/download.html)，查看最新发布的版本，截止到本教程完成时，其最新版本为 3.29.0，找到该版本的源码压缩包，复制其下载链接，然后通过 wget 命令下载到服务器（我一般习惯将源码放在 ~/src 目录下。）

```shell
# 创建 src 目录并进到这个目录
$ mkdir -p ~/src
$ cd ~/src

# 下载 sqlite3 源码并解压安装
$ wget https://sqlite.org/2019/sqlite-autoconf-3290000.tar.gz
$ tar zxvf sqlite-autoconf-3290000.tar.gz
$ cd sqlite-autoconf-3290000
$ ./configure
$ make
$ sudo make install
```

> 小贴士：
>
> 如果 wget 命令不存在，使用 sudo yum install -y wget 安装即可。

至此 SQLite3 更新完毕，接下来安装 Python3。

### 安装 Python3 和 Pipenv

CentOS 7 自带的 Python 发行版为 2.7，因此需要安装 Python3，为了兼容性，我们安装 Python 3.7.4。

首先安装可能的依赖：

```shell
$ sudo yum install -y openssl-devel bzip2-devel expat-devel gdbm-devel readline-devel sqlite-devel
```

然后下载 Python 3.6.4 的源码并解压：

```shell
$ cd ~/src
$ wget https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz
$ tar -zxvf Python-3.7.4.tgz
```

最后编译安装：

```shell
$ cd Python-3.7.4
$ ./co
nfigure LD_RUN_PATH=/usr/local/lib LDFLAGS="-L/usr/local/lib" CPPFLAGS="-I/usr/local/include"
$ make LD_RUN_PATH=/usr/local/lib
$ sudo make install
```

注意这里安装 Python 时，Python 会依赖 SQLite3 的库，所以在 configure 时通过 LD_RUN_PATH 指定依赖的搜索目录（因为我们之前更新了 SQLite3 的版本，指定依赖搜索目录确保使用新的 SQLite3 依赖库），另外两个参数作用类似。

然后输入 python3.7 -V 和 pip3.7 -V 命令测试安装结果，输出版本号说明安装成功了。

有了 pip，就可以安装 Pipenv 了：

```shell
$ sudo pip3.7 install pipenv
```

> 小贴士：
>
> 如果以上命令报错，可能是因为 pip3.7 安装在当前用户的 bin 路径下（/usr/local/bin/），使用 pip3.7 install pipenv --users 命令也把 Pipenv 安装到当前用户的 bin 路径下就可以了。

## 生产服务器部署

如果容器在本地运行没有问题了，线上环境的容器运行也没有问题，因为理论上，我们在线上服务器也会构建和本地测试用的容器一模一样的环境，所以几乎可以肯定，只要我们服务器有 Docker，那么我们的应用就可以成功运行。

### CentOS系统安装Docker以及相关配置

首先在服务安装 Docker，安装方式因系统而异，方式非常简单，我们以 CentOS 7 为例，其它系统请参考 [Docker 的官方文档](https://docs.docker.com/install/)。

首先安装必要依赖：

```shell
$ sudo yum install -y yum-utils \
  device-mapper-persistent-data \
  lvm2
```

然后添加仓库源：

```shell
$ sudo yum-config-manager \
    --add-repo \
    https://download.docker.com/linux/centos/docker-ce.repo
```

最后安装 Docker：

```shell
$ sudo yum install docker-ce docker-ce-cli containerd.io
```

启动 Docker：

```shell
$ sudo systemctl start docker
```

在 docker 中运行一个 hello world，确认 docker 安装成功：

```shell
$ sudo docker run hello-world
```

docker 安装成功了，还要安装一下 docker-compose。其实是一个 python 包，我们直接通过 pip 安装就可以了：

```shell
$ pip3 install docker-compose
```

为了避免运行一些 docker 命令时可能产生的权限问题，我们把系统当前用户加入到 docker 组里：

```shell
$ sudo usermod -aG docker ${USER}
```

添加组后要重启一下 shell（ssh 连接的话就断开重连）。

### Supervisor守护Docker容器进程

首先安装 Supervisor。注意这里使用的是**系统自带的 pip2**，因为截至本教程书写时 Supervisor 还不支持 Python3，不过这并不影响使用。

```shell
$ pip install supervisor
```

为了方便，我一般会设置如下的目录结构（位于 ~/etc 目录下）来管理 Supervisor 有关的文件：

```
~/etc
 
├── supervisor
│   ├── conf.d
│   └── var
│       ├── log
└── supervisord.conf
```

其中 supervisord.conf 是 Supervior 的配置文件，它会包含 conf.d 下的配置。var 目录下用于存放一些经常变动的文件，例如 socket 文件，pid 文件，log 下则存放日志文件。

首先来建立上述的目录结构：

```shell
$ mkdir -p ~/etc/supervisor/conf.d
$ mkdir -p ~/etc/supervisor/var/log
```

然后进入 ~/etc 目录下生成 Supervisor 的配置文件：

```shell
$ cd ~/etc
$ echo_supervisord_conf > supervisord.conf
```

修改 supervisor.conf，让 Supervisor 进程产生的一些文件生成到上面我们创建的目录下，而不是其默认指定的地方。

首先找到 [unix_http_server] 版块，将 file 设置改为如下的值：

```
[unix_http_server]
file=/home/bicycle/etc/supervisor/var/supervisor.sock
```

即让 socket 文件生成在 ~/etc/supervisor/var/ 目录下。注意 supervisor 不支持将 ~ 展开为用户 home 目录，所以要用绝对路径指定。

类似的修改 [supervisord] 板块下的 logfile 和 pidfile 文件的路径，还有 user 改为系统用户，这样 supervisor 启动的进程将以系统用户运行，避免可能的权限问题：

```
logfile=/home/bicycle/etc/supervisor/var/log/supervisord.log
pidfile=/home/bicycle/etc/supervisor/var/supervisord.pid
user=bicycle
```

还有 [supervisorctl] 板块下：

```
serverurl=unix:///home/bicycle/etc/supervisor/var/supervisor.sock
```

[include] 版块，将 /home/bicycle/etc/supervisor/conf.d/ 目录下所有以 .ini 结尾的文件内容包含到配置中来，这样便于配置的模块化管理，和之前 Nginx 配置文件的处理方式是类似的。

```
files = /home/bicycle/etc/supervisor/conf.d/*.ini
```

然后我们到 conf.d 新建我们博客应用的配置：

```
[program:vehiclebrand]
command=pipenv run gunicorn blogproject.wsgi -w 2 -k gthread -b 127.0.0.1:8000
directory=# by your self
autostart=true
autorestart=unexpected
user=bicycle
stdout_logfile=/home/bicycle/etc/supervisor/var/log/vehiclebrand.log
stderr_logfile=/home/bicycle/etc/supervisor/var/log/vehiclebrand.log
```

说一下各项配置的含义：

[program:hellodjango-blog-tutorial] 指明运行应用的进程，名为 hellodjango-blog-tutorial。

command 为进程启动时执行的命令。

directory 指定执行命令时所在的目录。

autostart 随 Supervisor 启动自动启动进程。

autorestart 进程意外退出时重启。

user 进程运行的用户，防止权限问题。

stdout_logfile，stderr_logfile 日志输出文件。

启动 Supervisor

```shell
$ supervisord -c ~/etc/supervisord.conf
```

-c 指定 Supervisr 启动时的配置文件。

进入 supervisorctl 进程管理控制台：

```shell
$ supervisorctl -c ~/etc/supervisord.conf
```

执行 update 命令更新配置文件并启动应用。

浏览器输入域名，可以看到服务已经正常启动了。

接下来拉取最新的代码到服务器，进入项目根目录下，创建线上环境需要的环境变量文件：

```bash
$ mkdir .envs
$ cd .envs
$ vi .production
```

将线上环境的 secret key 写入 .production 环境变量文件，

```
DJANGO_SECRET_KEY=2pe8eih8oah2_2z1=7f84bzme7^bwuto7y&f(#@rgd9ux9mp-3
```

保存并退出。

回到项目根目录，运行 build 命令构建镜像：

```bash
$ docker-compose -f production.yml up build
```

然后我们可以开始启动根据构建好的镜像启动 docker 容器，不过为了方便，我们的 docker 进程仍然由 supervisor 来管理，我们修改一下博客应用的配置，让它启动时启动 docker 容器。

打开 ~/etc/supervisor/conf.d/vehiclebrand.ini，修改为如下内容：

```
[program:vehiclebrand]
command=docker-compose -f production.yml up --build
directory=/home/bicycle/Applications/vehicleQrcodeWEB #注意自己更改自己地址
autostart=true
autorestart=unexpected
user=bicycle
stdout_logfile=/home/bicycle/etc/supervisor/var/log/vehiclebrand.log
stderr_logfile=/home/bicycle/etc/supervisor/var/log/vehiclebrand.log
```

主要就是把之前的使用 Gunicorn 来启动服务换成了启动 docker。

修改 ini 配置 要记得 reread 使配置生效：

```
$ supervisorctl -c ~/etc/supervisord.conf
> reread
> update 
```

docker 容器顺利启动，访问我们的项目网站。抛掉镜像编排的准备工作，相当于我们只执行了一条构建容器并启动容器的命令就部署了我们的博客应用。如果换台服务器，也只要再执行一下镜像构建和启动容器的命令，服务就又可以起来！这就是 docker 的好处。

访问docker 容器

```
docker exec -ti 52c08588ab5e sh
```
> 本项目在IDE VS code 软件进行开发，安装插件Docker 即可显示当前docker的镜像和容器。
