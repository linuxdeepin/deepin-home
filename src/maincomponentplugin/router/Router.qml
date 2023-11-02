// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: LGPL-3.0-or-later
// 全局的界面路由管理
// 用于在多个界面之前进行切换和导航，data参数作为属性传递给组件
// 在切换的过程中会记录路由历史，支持返回上一页

import QtQuick 2.0
pragma Singleton

Item {
    // 当前路由状态
    property var routeCurrent: routeIndex
    // 路由历史记录
    property var routeHistory: [routeIndex]
    // 首页
    property var routeIndex: {
        "path": "index",
        "component": "index/Index.qml",
        "data": {
            "showNotify": false
        }
    }
    // 我的反馈
    property var routeMyFeedback: {
        "path": "feedback/my",
        "component": "feedback/List.qml",
        "data": {
            "typeFilter": true,
            "relation": "create"
        }
    }
    // 反馈广场
    property var routeAllFeedback: {
        "path": "feedback/all",
        "component": "feedback/List.qml",
        "data": {
            "typeFilter": true
        }
    }
    // 我的收藏
    property var routeFavoriteFeedback: {
        "path": "feedback/collect",
        "component": "feedback/List.qml",
        "data": {
            "relation": "collect"
        }
    }
    // 我的关注
    property var routeUrgeFeedback: {
        "path": "feedback/like",
        "component": "feedback/List.qml",
        "data": {
            "relation": "like"
        }
    }
    // 反馈详情
    property var routeFeedbackDetail: {
        "path": "feedback/detail",
        "component": "feedback/Detail.qml"
    }
    // 网络错误
    property var routeNetworkError: {
        "path": "error/network",
        "component": "offline/Offline.qml"
    }

    // 路由后退信号
    signal signalBack()
    // 路由导航信号
    signal signalNavigate(var route, bool overlay)
    signal signalGoHome(var route)

    // 显示首页
    function showIndex() {
        routeCurrent = clone(routeIndex);
        routeHistory = [routeCurrent];
        signalGoHome(routeCurrent);
    }

    // 显示通知需要先返回首页
    function showNotify() {
        const r = clone(routeIndex);
        r.data = {
            "showNotify": true
        };
        routeCurrent = r;
        routeHistory = [r];
        signalGoHome(r);
    }

    // 显示反馈广场
    // push 为true时才会记录到路由，可以避免标签之前反复切换导致路由回退过多
    // type 类型筛选
    // overlay 为true时会覆盖上一个路由，避免路由切换丢失参数
    function showAllFeedback(overlay, type = "") {
        const r = clone(routeAllFeedback);
        r.data = {
            "typeFilter": true,
            "type": type
        };
        signalNavigate(r, overlay);
        routeCurrent = r;
        if (overlay) {
            routeHistory[routeHistory.length - 1] = r;
            return ;
        }
        routeHistory.push(routeCurrent);
    }

    // 显示我的反馈
    function showMyFeedback(overlay, type = "") {
        const r = clone(routeMyFeedback);
        r.data = {
            "typeFilter": true,
            "type": type,
            "relation": "create"
        };
        signalNavigate(r, overlay);
        routeCurrent = r;
        if (overlay) {
            routeHistory[routeHistory.length - 1] = r;
            return ;
        }
        routeHistory.push(routeCurrent);
    }

    // 显示我的收藏
    function showFavoriteFeedback(overlay = false) {
        routeCurrent = routeFavoriteFeedback;
        signalNavigate(routeCurrent, overlay);
        if (overlay)
            routeHistory[routeHistory.length - 1] = routeCurrent;
        else
            routeHistory.push(routeCurrent);
    }

    // 显示我的催促
    function showUrgeFeedback(overlay = false) {
        routeCurrent = routeUrgeFeedback;
        signalNavigate(routeCurrent, overlay);
        if (overlay)
            routeHistory[routeHistory.length - 1] = routeCurrent;
        else
            routeHistory.push(routeCurrent);
    }

    // 显示网络错误
    function showNetworkError() {
        routeCurrent = routeNetworkError;
        signalNavigate(routeCurrent, false);
        routeHistory.push(routeNetworkError);
    }

    // 显示反馈详情
    function showFeedbackDetail(feedback) {
        // 复制对象避免在回退时，拿到相同数据
        const value = clone(routeFeedbackDetail);
        value.data = {
            "feedback": feedback
        };
        routeCurrent = value;
        routeHistory.push(value);
        signalNavigate(routeCurrent, false);
    }

    // 复制对象
    function clone(route) {
        return JSON.parse(JSON.stringify(route));
    }

    // 返回上一页
    function back() {
        if (routeHistory.length == 1)
            return ;

        routeHistory.pop();
        const r = routeHistory[routeHistory.length - 1];
        console.log("route back", JSON.stringify(r));
        routeCurrent = r;
        signalBack();
    }

    // 调试
    onRouteCurrentChanged: {
        console.log("current route", routeCurrent.path);
    }
}
