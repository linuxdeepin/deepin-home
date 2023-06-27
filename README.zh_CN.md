# deepin-home

- [x] 接收官方新闻通知和调查问卷

- [x] 快速（单点）登录社区论坛

- [ ] 提交问题反馈和需求

- [ ] 社区论坛消息提醒

[英文文档](README.md) |
[设计文档](docs/design.zh_CN.md)

## 截图

![Screenshot](docs/screenshots/home_zh.webp)

## 运行依赖

- build-essential:amd64

## 构建依赖

- build-essential:amd64
- debhelper
- cmake
- qt5-default
- qtbase5-dev
- qtdeclarative5-dev
- libdtkcommon-dev
- libdtkcore-dev
- libdtkgui-dev
- libdtkdeclarative-dev
- qttools5-dev-tools
- qml-module-qtquick-dialogs

## 从源码构建

### 安装构建依赖

```shell
sudo apt build-dep deepin-home
```

### 调试构建

```shell
git clone https://github.com/linuxdeepin/deepin-home.git
cd deepin-home
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j
```

### 调试运行

```sh
# in the deepin-home/build/ directory
./src/main/deepin-home-daemon &
./src/main/deepin-home
```

### 手动安装

```sh
# in the deepin-home/build/ directory
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j && sudo make install
```

### deb打包

```sh
# in the deepin-home/ directory
dpkg-buildpackage -us -uc
```

## 获取帮助

- [官方论坛](https://bbs.deepin.org/)
- [开发者中心](https://github.com/linuxdeepin/developer-center)
- [Matrix](https://matrix.to/#/#deepin:matrix.org)
- [Wiki](https://wiki.deepin.org/)

## 参与

我们鼓励您参与问题报告和项目贡献

- [开发者代码贡献指南](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers)

## License

deepin-home 使用 [LGPL-3.0-or-later](LICENSE).
