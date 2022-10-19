import QtQuick 2.0

Item {
    
    function get(url, callback) {
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && xhr.status == 200) {
                callback(JSON.parse(xhr.responseText))
            }
        }
        xhr.open("GET", url, true)
        xhr.send()
    }
    function getNotify(callback) {
        get("http://10.0.33.45:12280/api/v1/public/channel/p/topic/n/messages?language=zh_CN&offset=0&limit=20", callback)
    }
    function getQuestionnaire(callback) {
        get("http://10.0.33.45:12280/api/v1/public/channel/p/topic/q/messages?language=zh_CN&offset=0&limit=20", callback)
    }
}