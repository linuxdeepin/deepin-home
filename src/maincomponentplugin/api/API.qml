pragma Singleton
import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Item {
    property string server: 'http://10.0.33.45:12280'
    property string language
    property var db

    function get(url, callback) {
        console.log("send get request", url)
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && xhr.status == 200) {
                callback(JSON.parse(xhr.responseText))
            }
        }
        xhr.open("GET",server + url, true)
        xhr.send()
    }
    function getLocalDB(callback) {
        if(db){
            callback(db)
        }else{
            var db = LocalStorage.openDatabaseSync("main", "1.0", "deepin home local storage", 1000000)
            db.transaction(tx => {
                tx.executeSql('CREATE TABLE IF NOT EXISTS keyvalue(key TEXT, value TEXT)');
                callback(db)
            })
        }
    }
    // 获取服务器判定语言（归类）
    function getLanguage(callback) {
        if (language) {
            callback(language)
        } else {
            get("/api/v1/public/language/zh_CN", (resp)=>{
                language = resp.code
                callback(language)
            })
        }
    }
    // 获取通知列表
    function getNotify(callback) {
        getLanguage(lang=>{
            get("/api/v1/public/channel/p/topic/n/messages?language="+lang+"&offset=0&limit=20", callback)
        })
    }
    // 获取调查问卷列表
    function getQuestionnaire(callback) {
        getLanguage(lang=>{
            get("/api/v1/public/channel/p/topic/q/messages?language="+lang+"&offset=0&limit=20", callback)
        })
    }
    // 获取内测渠道内容
    function getInternalTest(callback){
        getLanguage(lang=>{
            get("/api/v1/public/setting/internal-test_"+lang, callback)
        })
    }
    // 获取社区动态内容
    function getAboutUs(callback){
        getLanguage(lang=>{
            get("/api/v1/public/setting/aboutus_"+lang, callback)
        })
    }
    function markRead(uuid) {
        getLocalDB(db => {
            db.transaction(function (tx) {
                tx.executeSql('INSERT INTO keyvalue VALUES(?, ?)', [ uuid + "_read", "true" ]);
            })
        })
    }
    function isRead(uuid, callback) {
        getLocalDB(db => {
            db.transaction(function (tx) {
            var rs = tx.executeSql('SELECT value FROM keyvalue where key = ?', [uuid + "_read"]);
                callback(rs.rows.length>0)
            })
        })
    }
    Component.onCompleted: {
        getLanguage(lang => {})
        getLocalDB(db=>{})
    }
}