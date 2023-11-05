import x2js from 'x2js'

export function xmlToValue(xml) {
    xml = "<a>" + xml + "</a>";
    var x2j = new x2js();
    xml = x2j.xml2js(xml).a.data.value
    return xml.split(" ").filter((x) => x !== '');
}

export function xmlToObject(xml){
    xml = "<a>" + xml + "</a>";
    var x2j = new x2js();
    xml = x2j.xml2js(xml).a
    return xml
}

export function xmlToMat(xml){
    var result = "<a>" + xml + "</a>"
    var x2j = new x2js()
    result = x2j.xml2js(result).a
    return result
}

export function bindMouseDown(e, topBox) {
    var currentBox
    var resizeBox

    var startY
    var curHeight
    var topHeight

    resizeBox = e.target
    currentBox = e.target.parentNode// 当前盒子

    if (!topBox) return
    curHeight = currentBox.clientHeight
    topHeight = topBox.clientHeight
    // var otherBoxWidth = var $refs.mat.clientWidth - var currentBox.clientWidth - var rightBox.clientWidth // 其他盒子的宽度
    // 颜色改变提醒
    resizeBox.style.background = '#818181'
    startY = e.clientY
    document.addEventListener('mousemove', bindMousemove)
    document.addEventListener('mouseup', bindMouseup)

    //鼠标移动
    function bindMousemove(e) {
        const endY = e.clientY

        const moveLen = startY - endY // （startY - endY）= 移动的距离    //因为页面坐标轴里Y周是反方向，越往下越大
        const CurBoxLen = curHeight + moveLen  // 盒子起始位置.Y轴+移动的距离=下盒子区域最后的高度
        const topBoxLen = topHeight - moveLen    // 上盒子高度=总宽度-左侧宽度-其它盒子宽度
        //topBox.clientHeight - CurBoxLen
        // 当最小宽度时，无法继续拖动
        if (CurBoxLen <= 100 || topBoxLen <= 100) return
        currentBox.style.height = CurBoxLen + 'px'// 当前盒子的宽度
        topBox.style.height = topBoxLen + 'px'

    }
    //鼠标抬起
    function bindMouseup() {
        // 颜色恢复
        resizeBox.style.background = '#d6d6d6'
        document.removeEventListener('mousedown', bindMouseDown)
        document.removeEventListener('mousemove', bindMousemove)
    }

}