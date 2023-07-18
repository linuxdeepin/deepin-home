// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.

// SPDX-License-Identifier: LGPL-3.0-or-later

// 类似 js 的 promise + await

#include "promise.h"
MyPromise::MyPromise(QObject *parent)
    : QObject(parent)
{}
// 在异步处理完成后调用该函数，await会返回传递的result
void MyPromise::resolve(QVariant result)
{
    if (m_finish) {
        return;
    }
    m_finish = true;
    m_result = result;
    m_loop.quit();
};
// 在异步处理异常时调用该函数，await会抛出异常
void MyPromise::reject(QString err)
{
    if (m_finish) {
        return;
    }
    m_finish = true;
    m_error = err;
    m_loop.quit();
};

QVariant MyPromise::await()
{
    this->deleteLater();
    if (!m_finish) {
        m_loop.exec();
    }
    if (!m_error.isEmpty()) {
        qmlEngine(this)->throwError(QJSValue::GenericError, m_error);
    }
    return m_result;
};