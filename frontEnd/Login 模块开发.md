# Login 模块开发

## 前情提要

已经完成vehiclebrand项目，现阶段缺少对login模块的系统性构造。为了学习使用Django框架以及在后续的开发中代码复用login模块，需要在一定时间内完成本模块的学习和搭建任务。

vehiclebrand 项目地址：[Github](https://github.com/cycmay/vehicleQrcodeWEB)

login模块 学习博客地址：[博客](http://www.liujiangblog.com/course/django/102)

## 🧶 工具箱

### 开发环境

pycharm 可以识别出Pipenv虚拟环境，在preference可以看到设置的环境。

Django==3.0 latest

运行 `python manage.py startapp log` 创建login这个app。

### 注册app

每次创建了新的app后，都需要在全局settings中注册，这样Django才知道用户有新的应用上线了。在settings的下面部分添加‘login’，建议在最后添加个逗号。

```
# Application definition

INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'login',
]
```

### 数据库连接

Django中对数据库的设置在settings文件中，如下部分：

```
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': os.path.join(BASE_DIR, 'db.sqlite3'),
    }
}
```

Django默认使用SQLite数据库，并内置SQLite数据库的访问API，也就是说和Python一样原生支持SQLite。本项目使用SQLite作为后端数据库，因此不需要修改settings中这部分内容。

## 🌲 数据模型 

Django的设计分为三大块：MVT，M即为Model，是Django系统中的数据模型，也是连接数据库的数据表对象。通过数据模型，Django用户可以以面向对象的方式来访问数据库，此外，数据模型也是系统建设的重要环节，它体现了系统内数据表之间是否拥有良好的耦合性。是否有好的数据模型设计是反映开发者功底的一面。

### User 设计

在login目录下，Django系统会生成model.py文件，该文件是用来编写用户自定义的数据模型。

```python
from django.db import models

# Create your models here.


class User(models.Model):

    gender = (
        ('male', "男"),
        ('female', "女"),
    )

    name = models.CharField(max_length=128, unique=True)
    password = models.CharField(max_length=256)
    email = models.EmailField(unique=True)
    sex = models.CharField(max_length=32, choices=gender, default="男")
    c_time = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.name

    class Meta:
        ordering = ["-c_time"]
        verbose_name = "用户"
        verbose_name_plural = "用户"
```

各字段含义：

- name: 必填，最长不超过128个字符，并且唯一，也就是不能有相同姓名；
- password: 必填，最长不超过256个字符（实际可能不需要这么长）；
- email: 使用Django内置的邮箱类型，并且唯一；
- sex: 性别，使用了一个choice，只能选择男或者女，默认为男；
- 使用`__str__`方法帮助人性化显示对象信息；
- 元数据里定义用户按创建时间的反序排列，也就是最近的最先显示；

注意：这里的用户名指的是网络上注册的用户名，不要等同于现实中的真实姓名，所以采用了唯一机制。如果是现实中的人名，那是可以重复的，肯定是不能设置unique的。另外关于密码，建议至少128位长度，原因后面解释。

### 映射数据模型到数据表

app中的models建立好了后，并不会自动地在数据库中生成相应的数据表，需要手动创建。

进入Pycharm的terminal终端，执行下面的命令：

```
python manage.py makemigrations
```

返回结果：

```
Migrations for 'login':
  login\migrations\0001_initial.py
    - Create model User
```

Django自动为我们创建了`login\migrations\0001_initial.py`文件，保存了我们的第一次数据迁移工作，也就是创建了User模型。

接着执行下面的命令：

```
python manage.py migrate
```

Django将在数据库内创建真实的数据表。如果是第一次执行该命令，那么一些内置的框架，比如auth、session等的数据表也将被一同创建。

./manage makemigrations

will create (migration_number).py files in your app migrations folders. These lines of code are nothing but statements which help in creating actual fields in your respective database similar to SQL statements.

In order to execute the migration which was created using the previous command, we will run the following command,

`./manage.py migrate`

On migrate your new model fields will be reflected in database if there are no errors

### 注册模型App

admin后台本质上是Django给我们提供的一个app，默认情况下，它已经在settings中注册了，同样的还有session会话框架。

```
# Application definition

INSTALLED_APPS = [
    'django.contrib.admin',     # here
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',      # here
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'login',
]
```

进入`login/admin.py`文件，代码如下：

```
from django.contrib import admin

# Register your models here.

from . import models

admin.site.register(models.User)
```

暂时直接注册。

## 🦊 Django后台

### 创建超级管理员

Django的admin后台拥有完整的较为安全的用户认证和授权机制，防护等级还算可以。

要进入该后台，需要创建超级管理员，该管理员和我们先前创建的User用户不是一个概念，要注意区别对待。

同样在Pycharm的终端中，执行下面的命令：

```
python manage.py createsuperuser
```

用户名、邮箱和密码自行设定，但一定不要忘记。密码最好有一定强度，并且不能太简单和普遍，会有提示，我这里强制通过了。

```
用户名 (leave blank to use 'feixuelym'): admin
电子邮件地址: admin@admin.com
Password:
Password (again):
这个密码太常见了。
Bypass password validation and create user anyway? [y/N]: y
Superuser created successfully.
```

### 使用Admin后台

创建好超级管理员后，就可以启动开发服务器了，然后在浏览器中访问`http://127.0.0.1:8000/admin/`地址，可以看到登录界面。

输入先前创建的超级管理员账户，进入管理界面。

注意，下方的`认证和授权`是admin应用自身的账户管理，上面的LOGIN栏目才是之前创建的login应用所对应的User模型。

点击Login栏目中的用户链接，进入用户列表界面，发现是空的，因为当前没有任何用户。点击右上方的增加用户按钮，创建几个测试用户试试。

通过输入不同的数据，可以看到Email会有地址合法性检查，性别有个选择框，非常的人性化。

但是，实际上这里还有点小问题，那就是密码相关。密码不能保存为明文，其次，不可以这么随意的修改和设置密码。

## 📚 URL路由和视图设计

前面已经创建好数据模型了，并且在admin后台中添加了一些测试用户。下面就要设计好站点的url路由、对应的处理视图函数以及使用的前端模板了。

### 路由设计

我们初步设想需要下面的四个URL：

| URL        | 视图                 | 模板           | 说明 |
| ---------- | -------------------- | -------------- | ---- |
| /index/    | login.views.index    | index.html     | 主页 |
| /login/    | login.views.login    | login.html     | 登录 |
| /register/ | login.views.register | register.html  | 注册 |
| /logout/   | login.views.logout   | 无需专门的页面 | 登出 |

重要说明：由于本项目目的是打造一个针对管理系统、应用程序等需求下的可重用的登录/注册app，而不是门户网站、免费博客等无需登录即可访问的网站，所以在url路由、跳转策略和文件结构的设计上都是尽量自成体系。具体访问的策略如下：

- 未登录人员，不论是访问index还是login和logout，全部跳转到login界面
- 已登录人员，访问login会自动跳转到index页面
- 已登录人员，不允许直接访问register页面，需先logout
- 登出后，自动跳转到login界面

考虑到登录注册系统属于站点的一级功能，为了直观和更易于接受，这里没有采用二级路由的方式，而是在根路由下直接编写路由条目，同样也没有使用反向解析名（name参数）。所以，在重用本app的时候，一定要按照app使用说明，加入相应的url路由。

根据上面的策划，打开`project/urls.py`文件，写入下面的代码：

```python
from django.contrib import admin
from django.urls import path
from login import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('index/', views.index),
    path('login/', views.login),
    path('register/', views.register),
    path('logout/', views.logout),
]
```

注意要先从login导入views模块。

### 架构初步视图

路由写好了，就进入`login/views.py`文件编写视图的框架，代码如下：

```python
from django.shortcuts import render
from django.shortcuts import redirect

# Create your views here.


def index(request):
    pass
    return render(request, 'login/index.html')


def login(request):
    pass
    return render(request, 'login/login.html')


def register(request):
    pass
    return render(request, 'login/register.html')


def logout(request):
    pass
    return redirect("/login/")
```

暂时忽略视图内部的具体细节，而是把框架先搭建起来。

注意：

- 在顶部额外导入了`redirect`，用于logout后，页面重定向到‘/login/’这个url，当然你也可以重定向到别的页面；
- 另外三个视图都返回一个render调用，render方法接收request作为第一个参数，要渲染的页面为第二个参数，以及需要传递给页面的数据字典作为第三个参数（可以为空），表示根据请求的部分，以渲染的HTML页面为主体，使用模板语言将数据字典填入，然后返回给用户的浏览器。
- 渲染的对象为login目录下的html文件，这是一种安全可靠的文件组织方式，我们现在还没有创建这些文件。

###  创建HTML页面文件

在项目根路径的login目录中创建一个templates目录，再在templates目录里创建一个login目录。这么做有助于app复用，防止命名冲突，能更有效地组织大型工程。

在`login/templates/login`目录中创建三个文件`index.html`、`login.html`以及`register.html` ，并写入如下的代码：

**index.html:**

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>首页</title>
</head>
<body>
<h1>这仅仅是一个主页模拟！请根据实际情况接入正确的主页！</h1>
</body>
</html>
```

**login.html:**

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>登录</title>
</head>
<body>
<h1>登录页面</h1>
</body>
</html>
```

**register.html**:

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>注册</title>
</head>
<body>
<h1>注册页面</h1>
</body>
</html>
```

### 测试路由和视图

启动服务器，在浏览器访问`http://127.0.0.1:8000/index/`等页面，应能正常显示。

### 静态文件引入

在工程根目录下的login目录下，新建一个static目录，再到static目录里创建一个login目录，这种目录的创建方式和模板文件templates的创建方式都是一样的思维，也就是让重用app变得可能，并且不和其它的app发生文件路径和名称上的冲突。

继续在`/login/static/login`目录下创建一个css和一个image目录，css中添加我们为登录视图写的css文件，这里是`login.css`，image目录中存放css文件使用到的图片，示例：

下面我们修改一下login.html的代码，主要是引入了login.css文件，注意最开头的`{% load static %}`，表示我们要加载静态文件。

```html
{% load static %}
<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <!-- 上述meta标签*必须*放在最前面，任何其他内容都*必须*跟随其后！ -->
    <!-- Bootstrap CSS -->
    <link href="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet">
    <link href="{% static 'login/css/login.css' %}" rel="stylesheet"/>
    <title>登录</title>
  </head>
  <body>
    <div class="container">
            <div class="col">
              <form class="form-login" action="/login/" method="post">
                  <h3 class="text-center">欢迎登录</h3>
                  <div class="form-group">
                    <label for="id_username">用户名：</label>
                    <input type="text" name='username' class="form-control" id="id_username" placeholder="Username" autofocus required> <!-- here name should be cared-->
                  </div>
                  <div class="form-group">
                    <label for="id_password">密码：</label>
                    <input type="password" name='password' class="form-control" id="id_password" placeholder="Password" required>
                  </div>
                  <div>
                  <a href="/register/" class="text-success "><ins>新用户注册</ins></a>
                  <button type="submit" class="btn btn-primary float-right">登录</button>
                  </div>
              </form>
            </div>
    </div> <!-- /container -->

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    {#    以下三者的引用顺序是固定的#}
    <script src="https://cdn.bootcss.com/jquery/3.3.1/jquery.js"></script>
    <script src="https://cdn.bootcss.com/popper.js/1.15.0/umd/popper.js"></script>
    <script src="https://cdn.bootcss.com/twitter-bootstrap/4.3.1/js/bootstrap.min.js"></script>

  </body>
</html>
```

而login.css文件的代码如下，注意其中背景图片bg.jpg的引用方式：

```css
body {
  height: 100%;
  background-image: url('../image/bg.jpg'); /* css的相对路径*/
}
```

## 🐰 login视图

### 登录视图

根据我们在路由中的设计，用户通过`login.html`中的表单填写用户名和密码，并以POST的方式发送到服务器的`/login/`地址。服务器通过`login/views.py`中的`login()`视图函数，接收并处理这一请求。

我们可以通过下面的方法接收和处理请求：

```python
def login(request):
    if request.method == "POST":
        username = request.POST.get('username')
        password = request.POST.get('password')
        print(username, password)
        return redirect('/index/')
    return render(request, 'login/login.html')
```

说明：

- 每个视图函数都至少接收一个参数，并且是第一位置参数，该参数封装了当前请求的所有数据；
- 通常将第一参数命名为request，当然也可以是别的；
- `request.method`中封装了数据请求的方法，如果是“POST”（全大写），将执行if语句的内容，如果不是，直接返回最后的render()结果，也就是正常的登录页面；
- `request.POST`封装了所有POST请求中的数据，这是一个字典类型，可以通过get方法获取具体的值。
- 类似`get('username')`中的键‘username’是HTML模板中表单的input元素里‘name’属性定义的值。所以在编写form表单的时候一定不能忘记添加name属性。
- 利用print函数在开发环境中验证数据；
- 利用redirect方法，将页面重定向到index页。

启动服务器，然后在`http://127.0.0.1:8000/login/`的表单中随便填入用户名和密码，然后点击提交。然而，页面却出现了错误提示，如下所示：

错误是CSRF验证失败，请求被中断。CSRF（Cross-site request forgery）跨站请求伪造，是一种常见的网络攻击手段，具体原理和技术内容请自行百科。Django自带对许多常见攻击手段的防御机制，CSRF就是其中一种，还有XSS、SQL注入等。

解决这个问题的办法其实在Django的Debug错误页面已经给出了，我们需要在前端页面的form表单内添加一个`{% csrf_token %}`标签。这个标签必须放在form表单内部，但是内部的位置可以随意。

### 验证用户

通过唯一的用户名，使用Django的ORM去数据库中查询用户数据，如果有匹配项，则进行密码对比，如果没有匹配项，说明用户名不存在。如果密码对比错误，说明密码不正确。

```python
def login(request):
    if request.method == "POST":
        username = request.POST.get('username')
        password = request.POST.get('password')
        if username.strip() and password:  # 确保用户名和密码都不为空
            # 用户名字符合法性验证
            # 密码长度验证
            # 更多的其它验证.....
            try:
                user = models.User.objects.get(name=username)
            except:
                return render(request, 'login/login.html')
            if user.password == password:
                return redirect('/index/')
    return render(request, 'login/login.html')
```

- 首先要在顶部导入models模块；
- 使用try异常机制，防止数据库查询失败的异常；
- 如果未匹配到用户，则执行except中的语句；注意这里没有区分异常的类型，因为在数据库访问过程中，可能发生很多种类型的异常，我们要对用户屏蔽这些信息，不可以暴露给用户，而是统一返回一个错误提示，比如用户名不存在。这是大多数情况下的通用做法。
- `models.User.objects.get(name=username)`是Django提供的最常用的数据查询API，具体含义和用法可以阅读前面的章节，不再赘述；
- 通过`user.password == password`进行密码比对，成功则跳转到index页面，失败则返回登录页面。

上面的代码还缺少很重要的一部分内容，也就是错误提示信息！无论是登录成功还是失败，用户都没有得到任何提示信息，这显然是不行的。

修改login视图：

```python
def login(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        message = '请检查填写的内容！'
        if username.strip() and password:
            # 用户名字符合法性验证
            # 密码长度验证
            # 更多的其它验证.....
            try:
                user = models.User.objects.get(name=username)
            except :
                message = '用户不存在！'
                return render(request, 'login/login.html', {'message': message})

            if user.password == password:
                print(username, password)
                return redirect('/index/')
            else:
                message = '密码不正确！'
                return render(request, 'login/login.html', {'message': message})
        else:
            return render(request, 'login/login.html', {'message': message})
    return render(request, 'login/login.html')
```

请仔细分析一下上面的登录和密码验证逻辑，以及错误提示的安排。

这里增加了message变量，用于保存提示信息。当有错误信息的时候，将错误信息打包成一个字典，然后作为第三个参数提供给render方法。这个数据字典在渲染模板的时候会传递到模板里供你调用。

为了在前端页面显示信息，还需要对`login.html`进行修改：

```html
<form class="form-login" action="/login/" method="post">
                  {% if message %}
                    <div class="alert alert-warning">{{ message }}</div>
                  {% endif %}
                  {% csrf_token %}
                  <h3 class="text-center">欢迎登录</h3>
                  <div class="form-group">
                    <label for="id_username">用户名：</label>
                    <input type="text" name='username' class="form-control" id="id_username" placeholder="Username" autofocus required>
                  </div>
                  <div class="form-group">
                    <label for="id_password">密码：</label>
                    <input type="password" name='password' class="form-control" id="id_password" placeholder="Password" required>
                  </div>
                  <div>
                  <a href="/register/" class="text-success " ><ins>新用户注册</ins></a>
                  <button type="submit" class="btn btn-primary float-right">登录</button>
                  </div>
                </form>
```

Django的模板语言`{% if xxx %}{% endif %}`非常类似Python的if语句，也可以添加`{% else %}`分句。例子中，通过判断message变量是否为空，也就是是否有错误提示信息，如果有，就显示出来。

## 🐈 login 表单

Django在内部集成了一个表单功能，以面向对象的方式，直接使用Python代码生成HTML表单代码，专门帮助我们快速处理表单相关的内容。

Django的表单给我们提供了下面三个主要功能：

- 准备和重构数据用于页面渲染；
- 为数据创建HTML表单元素；
- 接收和处理用户从表单发送过来的数据。

### 编写登录表单

编写Django的form表单，非常类似我们在模型系统里编写一个模型。在模型中，一个字段代表数据表的一列，而form表单中的一个字段代表`<form>`中的一个`<input>`元素。

在`/login/forms.py`中写入下面的代码：

```python
from django import forms


class UserForm(forms.Form):
    username = forms.CharField(label="用户名", max_length=128)
    password = forms.CharField(label="密码", max_length=256, widget=forms.PasswordInput)
```

说明：

- 顶部要先导入forms模块
- 所有的表单类都要继承forms.Form类
- 每个表单字段都有自己的字段类型比如CharField，它们分别对应一种HTML语言中`<form>`内的一个input元素。这一点和Django模型系统的设计非常相似。
- label参数用于设置`<label>`标签
- `max_length`限制字段输入的最大长度。它同时起到两个作用，一是在浏览器页面限制用户输入不可超过字符数，二是在后端服务器验证用户输入的长度也不可超过。
- `widget=forms.PasswordInput`用于指定该字段在form表单里表现为`<input type='password' />`，也就是密码输入框。

### 修改视图

```python
def login(request):
    if request.method == 'POST':
        login_form = forms.UserForm(request.POST)
        message = '请检查填写的内容！'
        if login_form.is_valid():
            username = login_form.cleaned_data.get('username')
            password = login_form.cleaned_data.get('password')

            try:
                user = models.User.objects.get(name=username)
            except :
                message = '用户不存在！'
                return render(request, 'login/login.html', locals())

            if user.password == password:
                return redirect('/index/')
            else:
                message = '密码不正确！'
                return render(request, 'login/login.html', locals())
        else:
            return render(request, 'login/login.html', locals())

    login_form = forms.UserForm()
    return render(request, 'login/login.html', locals())
```

说明：

- 在顶部要导入我们写的forms模块:`from . import forms`
- 对于非POST方法发送数据时，比如GET方法请求页面，返回空的表单，让用户可以填入数据；
- 对于POST方法，接收表单数据，并验证；
- 使用表单类自带的`is_valid()`方法一步完成数据验证工作；
- 验证成功后可以从表单对象的`cleaned_data`数据字典中获取表单的具体值；
- 如果验证不通过，则返回一个包含先前数据的表单给前端页面，方便用户修改。也就是说，它会帮你保留先前填写的数据内容，而不是返回一个空表！

另外，这里使用了一个小技巧，Python内置了一个locals()函数，它返回当前所有的本地变量字典，我们可以偷懒的将这作为render函数的数据字典参数值，就不用费劲去构造一个形如`{'message':message, 'login_form':login_form}`的字典了。这样做的好处当然是大大方便了我们，但是同时也可能往模板传入了一些多余的变量数据，造成数据冗余降低效率。

### 修改前端代码

```html
<div class="container">
            <div class="col">
                <form class="form-login" action="/login/" method="post">
                  {% if message %}
                    <div class="alert alert-warning">{{ message }}</div>
                  {% endif %}
                  {% csrf_token %}
                  <h3 class="text-center">欢迎登录</h3>

                  {{ login_form }}

                  <div>
                      <a href="/register/" class="text-success " ><ins>新用户注册</ins></a>
                      <button type="submit" class="btn btn-primary float-right">登录</button>
                  </div>
                </form>
            </div>
    </div> <!-- /container -->
```

一个`{{ login_form }}`就直接完成了表单内容的生成工作！`login_form`这个名称来自你在视图函数中生成的form实例的变量名！

实际上除了通过`{{ login_form }}`简单地将表单渲染到HTML页面中了，还有下面几种方式：

- `{{ login_form.as_table }}` 将表单渲染成一个表格元素，每个输入框作为一个`<tr>`标签
- `{{ login_form.as_p }}` 将表单的每个输入框包裹在一个`<p>`标签内
- `{{ login_form.as_ul }}` 将表单渲染成一个列表元素，每个输入框作为一个`<li>`标签

注意：上面的渲染方法中都要自己手动编写`<table>`或者`<ul>`标签。

### 手动渲染表单字段

直接`{{ login_form }}`虽然好，但是无法用户自定义样式。可以通过手动渲染表单字段来操纵

可以通过`{{ login_form.name_of_field }}`获取每一个字段，然后分别渲染，如下例所示：

```html
<div class="form-group">
  {{ login_form.username.label_tag }}
  {{ login_form.username}}
</div>
<div class="form-group">
  {{ login_form.password.label_tag }}
  {{ login_form.password }}
</div>
```

其中的label标签可以用`label_tag`方法来生成。这样子更加灵活了,但是灵活的代价就是我们要写更多的代码，又偏向原生的HTML代码多了一点。

表单增加css属性：

```python
class UserForm(forms.Form):
    username = forms.CharField(label='用户名', max_length=128, widget=forms.TextInput(attrs={'class': 'form-control'}))
    password = forms.CharField(label='密码', max_length=256, widget=forms.PasswordInput(attrs={'class': 'form-control'}))
    #captcha = CaptchaField(label='验证码')
```

attrs 就是给表单元素增加属性

## 📷 图片验证码

为了防止机器人频繁登录网站或者破坏分子恶意登录，很多用户登录和注册系统都提供了图形验证码功能。

验证码（CAPTCHA）是“Completely Automated Public Turing test to tell Computers and Humans Apart”（全自动区分计算机和人类的图灵测试）的缩写，是一种区分用户是计算机还是人的公共全自动程序。可以防止恶意破解密码、刷票、论坛灌水，有效防止某个黑客对某一个特定注册用户用特定程序暴力破解方式进行不断的登陆尝试。

图形验证码的历史比较悠久，到现在已经有点英雄末路的味道了。因为机器学习、图像识别的存在，机器人已经可以比较正确的识别图像内的字符了。但不管怎么说，作为一种防御手段，至少还是可以抵挡一些低级入门的攻击手段，抬高了攻击者的门槛。

在Django中实现图片验证码功能非常简单，有现成的第三方库可以使用，我们不必自己开发（也要能开发得出来，囧）。这个库叫做`django-simple-captcha`。

### 安装captcha

在Pycharm的terminal中，使用pip安装第三方库：

```sh
执行命令：pip install django-simple-captcha
$ pip install django-simple-captcha
Collecting django-simple-captcha
  Downloading https://files.pythonhosted.org/packages/86/d4/5baf10bfc9eb7844872c256898a405e81f22f7213e008ec90875689f913d/django-simple-captcha-0
.5.11.zip (234kB)
    100% |████████████████████████████████| 235kB 596kB/s
Collecting six>=1.2.0 (from django-simple-captcha)
  Downloading https://files.pythonhosted.org/packages/73/fb/00a976f728d0d1fecfe898238ce23f502a721c0ac0ecfedb80e0d88c64e9/six-1.12.0-py2.py3-none
-any.whl
Requirement already satisfied: Django>=1.8 in d:\work\2019\for_test\mysite\venv\lib\site-packages (from django-simple-captcha) (2.2)
Collecting Pillow!=5.1.0,>=2.2.2 (from django-simple-captcha)
  Downloading https://files.pythonhosted.org/packages/40/f2/a424d4d5dd6aa8c26636969decbb3da1c01286d344e71429b1d648bccb64/Pillow-6.0.0-cp37-cp37m
-win_amd64.whl (2.0MB)
    100% |████████████████████████████████| 2.0MB 2.2MB/s
Collecting django-ranged-response==0.2.0 (from django-simple-captcha)
  Downloading https://files.pythonhosted.org/packages/70/e3/9372fcdca8e9c3205e7979528ccd1a14354a9a24d38efff11c1846ff8bf1/django-ranged-response-
0.2.0.tar.gz
Requirement already satisfied: sqlparse in d:\work\2019\for_test\mysite\venv\lib\site-packages (from Django>=1.8->django-simple-captcha) (0.3.0)

Requirement already satisfied: pytz in d:\work\2019\for_test\mysite\venv\lib\site-packages (from Django>=1.8->django-simple-captcha) (2018.9)
Installing collected packages: six, Pillow, django-ranged-response, django-simple-captcha
  Running setup.py install for django-ranged-response ... done
  Running setup.py install for django-simple-captcha ... done
Successfully installed Pillow-6.0.0 django-ranged-response-0.2.0 django-simple-captcha-0.5.11 six-1.12.0
```

pip自动帮我们安装了相关的依赖库`six`、`olefile`和`Pillow`，其中的Pillow是大名鼎鼎的绘图模块。

### 注册captcha

在settings中，将‘captcha’注册到app列表里：

```python
# Application definition

INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'login',
    'captcha',
]
```

captcha需要在数据库中建立自己的数据表，所以需要执行migrate命令生成数据表：

```sh
$ python manage.py migrate
Operations to perform:
  Apply all migrations: admin, auth, captcha, contenttypes, login, sessions
Running migrations:
  Applying captcha.0001_initial... OK
```

### 添加url路由

在根目录下的urls.py文件中增加captcha对应的url：

```python
from django.contrib import admin
from django.urls import path
from django.urls import include
from login import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('index/', views.index),
    path('login/', views.login),
    path('register/', views.register),
    path('logout/', views.logout),
    path('captcha/', include('captcha.urls'))   # 增加这一行
]
```

由于使用了二级路由机制，需要在顶部`from django.urls import include`。

### 修改forms.py

如果上面都OK了，就可以直接在我们的forms.py文件中添加CaptchaField了。

```python
from django import forms
from captcha.fields import CaptchaField

class UserForm(forms.Form):
    username = forms.CharField(label="用户名", max_length=128, widget=forms.TextInput(attrs={'class': 'form-control', 'placeholder': "Username",'autofocus': ''}))
    password = forms.CharField(label="密码", max_length=256, widget=forms.PasswordInput(attrs={'class': 'form-control',  'placeholder': "Password"}))
    captcha = CaptchaField(label='验证码')
```

注意需要提前导入`from captcha.fields import CaptchaField`，然后就像写普通的form字段一样添加一个captcha字段就可以了！

### 修改login.html

由于我们前面是手动生成的form表单，所以还要修改一下，添加captcha的相关内容，如下所示：

```html

            <div class="col">
                <form class="form-login" action="/login/" method="post">

                {% if login_form.captcha.errors %}
                    <div class="alert alert-warning">{{ login_form.captcha.errors }}</div>
                {% elif message %}
                    <div class="alert alert-warning">{{ message }}</div>
                {% endif %}

                  {% csrf_token %}
                  <h3 class="text-center">欢迎登录</h3>

                  <div class="form-group">
                    {{ login_form.username.label_tag }}
                    {{ login_form.username}}
                  </div>

                  <div class="form-group">
                    {{ login_form.password.label_tag }}
                    {{ login_form.password }}
                  </div>

                  <div class="form-group">
                    {{ login_form.captcha.label_tag }}
                    {{ login_form.captcha }}
                  </div>

                  <div>
                      <a href="/register/" class="text-success " ><ins>新用户注册</ins></a>
                      <button type="submit" class="btn btn-primary float-right">登录</button>
                  </div>
                </form>
            </div>
    </div> <!-- /container -->
```

这里在顶部的消息处，在`{% if %}`模板代码中，额外增加了一条`{{ login_form.captcha.errors }}`的判断，用于明确指示用户的验证码不正确。

## 👀 session会话

因为因特网HTTP协议的特性，每一次来自于用户浏览器的请求（request）都是无状态的、独立的。通俗地说，就是无法保存用户状态，后台服务器根本就不知道当前请求和以前及以后请求是否来自同一用户。对于静态网站，这可能不是个问题，而对于动态网站，尤其是京东、天猫、银行等购物或金融网站，无法识别用户并保持用户状态是致命的，根本就无法提供服务。你可以尝试将浏览器的cookie功能关闭，你会发现将无法在京东登录和购物。

为了实现连接状态的保持功能，网站会通过用户的浏览器在用户机器内被限定的硬盘位置中写入一些数据，也就是所谓的Cookie。通过Cookie可以保存一些诸如用户名、浏览记录、表单记录、登录和注销等各种数据。但是这种方式非常不安全，因为Cookie保存在用户的机器上，如果Cookie被伪造、篡改或删除，就会造成极大的安全威胁，因此，现代网站设计通常将Cookie用来保存一些不重要的内容，实际的用户数据和状态还是以Session会话的方式保存在服务器端。

但是，必须清楚的是**Session依赖Cookie**！不同的地方在于Session将所有的数据都放在服务器端，用户浏览器的Cookie中只会保存一个非明文的识别信息，比如哈希值。

Django提供了一个通用的Session框架，并且可以使用多种session数据的保存方式：

- 保存在数据库内
- 保存到缓存
- 保存到文件内
- 保存到cookie内

通常情况，没有特别需求的话，请使用保存在数据库内的方式，尽量不要保存到Cookie内。

Django的session框架默认启用，并已经注册在app设置内，如果真的没有启用，那么参考下面的内容添加有说明的那两行，再执行migrate命令创建数据表，就可以使用session了。

```python
# Application definition

INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',    # 这一行
    'django.contrib.messages',
    'django.contrib.staticfiles',
]

MIDDLEWARE = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',  # 这一行
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
]
```

当session启用后，传递给视图request参数的HttpRequest对象将包含一个session属性，就像一个字典对象一样。你可以在Django的任何地方读写`request.session`属性，或者多次编辑使用它。

下面是session使用参考：

```python
class backends.base.SessionBase
        # 这是所有会话对象的基类，包含标准的字典方法:
        __getitem__(key)
            Example: fav_color = request.session['fav_color']
        __setitem__(key, value)
            Example: request.session['fav_color'] = 'blue'
        __delitem__(key)
            Example: del request.session['fav_color']  # 如果不存在会抛出异常
        __contains__(key)
            Example: 'fav_color' in request.session
        get(key, default=None)
            Example: fav_color = request.session.get('fav_color', 'red')
        pop(key, default=__not_given)
            Example: fav_color = request.session.pop('fav_color', 'blue')
        # 类似字典数据类型的内置方法
        keys()
        items()
        setdefault()
        clear()


        # 它还有下面的方法：
        flush()
            # 删除当前的会话数据和会话cookie。经常用在用户退出后，删除会话。

        set_test_cookie()
            # 设置一个测试cookie，用于探测用户浏览器是否支持cookies。由于cookie的工作机制，你只有在下次用户请求的时候才可以测试。
        test_cookie_worked()
            # 返回True或者False，取决于用户的浏览器是否接受测试cookie。你必须在之前先调用set_test_cookie()方法。
        delete_test_cookie()
            # 删除测试cookie。
        set_expiry(value)
            # 设置cookie的有效期。可以传递不同类型的参数值：
        • 如果值是一个整数，session将在对应的秒数后失效。例如request.session.set_expiry(300) 将在300秒后失效.
        • 如果值是一个datetime或者timedelta对象, 会话将在指定的日期失效
        • 如果为0，在用户关闭浏览器后失效
        • 如果为None，则将使用全局会话失效策略
        失效时间从上一次会话被修改的时刻开始计时。

        get_expiry_age()
            # 返回多少秒后失效的秒数。对于没有自定义失效时间的会话，这等同于SESSION_COOKIE_AGE.
            # 这个方法接受2个可选的关键字参数
        • modification:会话的最后修改时间（datetime对象）。默认是当前时间。
        •expiry: 会话失效信息，可以是datetime对象，也可以是int或None

        get_expiry_date()
            # 和上面的方法类似，只是返回的是日期

        get_expire_at_browser_close()
            # 返回True或False，根据用户会话是否是浏览器关闭后就结束。

        clear_expired()
            # 删除已经失效的会话数据。
        cycle_key()
            # 创建一个新的会话秘钥用于保持当前的会话数据。django.contrib.auth.login() 会调用这个方法。
```

基本上背下来上面的内容，Django的session你就可以信手拈来了。

### 使用session

下面结合我们的项目实战，使用session。

首先，修改`login/views.py`中的login()视图函数：

```python
def login(request):
    if request.session.get('is_login', None):  # 不允许重复登录
        return redirect('/index/')
    if request.method == 'POST':
        login_form = forms.UserForm(request.POST)
        message = '请检查填写的内容！'
        if login_form.is_valid():
            username = login_form.cleaned_data.get('username')
            password = login_form.cleaned_data.get('password')

            try:
                user = models.User.objects.get(name=username)
            except :
                message = '用户不存在！'
                return render(request, 'login/login.html', locals())

            if user.password == password:
                request.session['is_login'] = True
                request.session['user_id'] = user.id
                request.session['user_name'] = user.name
                return redirect('/index/')
            else:
                message = '密码不正确！'
                return render(request, 'login/login.html', locals())
        else:
            return render(request, 'login/login.html', locals())

    login_form = forms.UserForm()
    return render(request, 'login/login.html', locals())
```

通过下面的if语句，我们不允许重复登录：

```python
if request.session.get('is_login',None):
    return redirect("/index/")
```

通过下面的语句，我们往session字典内写入用户状态和数据：

```python
request.session['is_login'] = True
request.session['user_id'] = user.id
request.session['user_name'] = user.name
```

你完全可以往里面写任何数据，不仅仅限于用户相关！

既然有了session记录用户登录状态，那么就可以完善我们的登出视图函数了：

```python
def logout(request):
    if not request.session.get('is_login', None):
        # 如果本来就未登录，也就没有登出一说
        return redirect("/login/")
    request.session.flush()
    # 或者使用下面的方法
    # del request.session['is_login']
    # del request.session['user_id']
    # del request.session['user_name']
    return redirect("/login/")
```

flush()方法是比较安全的一种做法，而且一次性将session中的所有内容全部清空，确保不留后患。但也有不好的地方，那就是如果你在session中夹带了一点‘私货’，会被一并删除，这一点一定要注意。

### 在index页面中验证登录

有了用户状态，就可以根据用户登录与否，展示不同的页面，比如在index页面中显示当前用户的名称：

修改index.html的代码：

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>首页</title>
</head>
<body>
<h1>{{ request.session.user_name }}!  欢迎回来！</h1>
<p>
    <a href="/logout/">登出</a>
</p>
</body>
</html>
```

注意其中的模板语言，`{{ request }}`这个变量会被默认传入模板中，可以通过圆点的调用方式，获取它内部的`{{ request.session }}`，再进一步的获取session中的内容。其实`{{ request }}`中的数据远不止此，例如`{{ request.path }}`就可以获取先前的url地址。

在已经login的状态下，手动从浏览器地址栏中访问/login/也依然进入的是index页面。在logout的状态下，都会跳转到login页面。但是，需要注意的是，我们目前还没有编写index未登录限制访问的代码。

修改index视图函数，添加相关限制：

```python
def index(request):
    if not request.session.get('is_login', None):
        return redirect('/login/')
    return render(request, 'login/index.html')
```

## 👱 注册视图

注册用户

### 创建forms

显而易见，我们的注册页面也需要一个form表单。同样地，在`/login/forms.py`中添加一个新的表单类：

```python
class RegisterForm(forms.Form):
    gender = (
        ('male', "男"),
        ('female', "女"),
    )
    username = forms.CharField(label="用户名", max_length=128, widget=forms.TextInput(attrs={'class': 'form-control'}))
    password1 = forms.CharField(label="密码", max_length=256, widget=forms.PasswordInput(attrs={'class': 'form-control'}))
    password2 = forms.CharField(label="确认密码", max_length=256, widget=forms.PasswordInput(attrs={'class': 'form-control'}))
    email = forms.EmailField(label="邮箱地址", widget=forms.EmailInput(attrs={'class': 'form-control'}))
    sex = forms.ChoiceField(label='性别', choices=gender)
    #captcha = CaptchaField(label='验证码')
```

说明：

- gender字典和User模型中的一样，其实可以拉出来作为常量共用，为了直观，特意重写一遍；
- password1和password2，用于输入两遍密码，并进行比较，防止误输密码；
- email是一个邮箱输入框；
- sex是一个select下拉框；
- 没有添加更多的input属性

### 完善register.html

同样地，类似login.html文件，我们手工在register.html中编写forms相关条目：

```html

    <div class="container">
            <div class="col">
                <form class="form-register" action="/register/" method="post">

                    {% if register_form.captcha.errors %}
                        <div class="alert alert-warning">{{ register_form.captcha.errors }}</div>
                    {% elif message %}
                        <div class="alert alert-warning">{{ message }}</div>
                    {% endif %}

                  {% csrf_token %}
                  <h3 class="text-center">欢迎注册</h3>

                  <div class="form-group">
                      {{ register_form.username.label_tag }}
                      {{ register_form.username}}
                  </div>
                  <div class="form-group">
                      {{ register_form.password1.label_tag }}
                      {{ register_form.password1 }}
                  </div>
                  <div class="form-group">
                      {{ register_form.password2.label_tag }}
                      {{ register_form.password2 }}
                  </div>
                  <div class="form-group">
                      {{ register_form.email.label_tag }}
                      {{ register_form.email }}
                  </div>
                  <div class="form-group">
                      {{ register_form.sex.label_tag }}
                      {{ register_form.sex }}
                  </div>
                  <div class="form-group">
                      {{ register_form.captcha.label_tag }}
                      {{ register_form.captcha }}
                  </div>

                  <div>
                      <a href="/login/"  ><ins>直接登录</ins></a>
                      <button type="submit" class="btn btn-primary float-right">注册</button>
                  </div>
                </form>
            </div>
    </div> <!-- /container -->

```

需要注意的是：

- form标签的action地址为`/register/`，class为`form-register`
- from中传递过来的表单变量名字为`register_form`
- 最下面的链接修改为直接登录的链接

### 实现注册视图

进入`/login/views.py`文件，现在来完善我们的`register()`视图：

```python
def register(request):
    if request.session.get('is_login', None):
        return redirect('/index/')

    if request.method == 'POST':
        register_form = forms.RegisterForm(request.POST)
        message = "请检查填写的内容！"
        if register_form.is_valid():
            username = register_form.cleaned_data.get('username')
            password1 = register_form.cleaned_data.get('password1')
            password2 = register_form.cleaned_data.get('password2')
            email = register_form.cleaned_data.get('email')
            sex = register_form.cleaned_data.get('sex')

            if password1 != password2:
                message = '两次输入的密码不同！'
                return render(request, 'login/register.html', locals())
            else:
                same_name_user = models.User.objects.filter(name=username)
                if same_name_user:
                    message = '用户名已经存在'
                    return render(request, 'login/register.html', locals())
                same_email_user = models.User.objects.filter(email=email)
                if same_email_user:
                    message = '该邮箱已经被注册了！'
                    return render(request, 'login/register.html', locals())

                new_user = models.User()
                new_user.name = username
                new_user.password = password1
                new_user.email = email
                new_user.sex = sex
                new_user.save()

                return redirect('/login/')
        else:
            return render(request, 'login/register.html', locals())
    register_form = forms.RegisterForm()
    return render(request, 'login/register.html', locals())
```

从大体逻辑上，也是先实例化一个RegisterForm的对象，然后使用`is_valide()`验证数据，再从`cleaned_data`中获取数据。

重点在于注册逻辑，首先两次输入的密码必须相同，其次不能存在相同用户名和邮箱，最后如果条件都满足，利用ORM的API，创建一个用户实例，然后保存到数据库内。

对于注册的逻辑，不同的生产环境有不同的要求，请跟进实际情况自行完善，这里只是一个基本的注册过程，不能生搬照抄。

### 密码加密

本项目到现在都还一直在用明文的密码！

对于如何加密密码，有很多不同的途径，其安全程度也高低不等。这里我们使用Python内置的hashlib库，使用哈希值的方式加密密码，可能安全等级不够高，但足够简单，方便使用。

首先在`login/views.py`中编写一个hash函数：

```
import hashlib

def hash_code(s, salt='mysite'):# 加点盐
    h = hashlib.sha256()
    s += salt
    h.update(s.encode())  # update方法只接收bytes类型
    return h.hexdigest()
```

使用了sha256算法，加了点盐。

然后，我们还要对login()和register()视图进行一下修改：

```
 if user.password == hash_code(password):
```