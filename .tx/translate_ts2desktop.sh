#! /bin/bash

# SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

# Use project root as working dir
cd "$(dirname "$0")/../" || exit

# Do convert
deepin-desktop-ts-convert ts2desktop ./deepin-home.desktop translations/deepin-home-desktop deepin-home.desktop