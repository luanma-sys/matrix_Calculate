<template>
  <div style="height:100vh">
    <el-tabs v-model="activeName" class="demo-tabs">
      <el-tab-pane label="线性模型" name="first"></el-tab-pane>
    </el-tabs>
    <div ref="mat" class="matrixbox contentmat" style="height:70%">
      <div id="demo" style="width:100%;height:500"></div>
    </div>
    <div>
      <el-button @click="mat_Request">计算结果</el-button>
      <el-button @click="addScatter">增加坐标点</el-button>
      <span style="margin-left:10px;width:200px;">
        xAixs:&nbsp; <span v-show="!coordinate.xAixs.edit" style="width:200px">({{ coordinate.xAixs.boundary[0].value
        }},{{ coordinate.xAixs.boundary[1].value }})</span>
        <el-input style="width:100px" v-show="coordinate.xAixs.edit"
          v-model="coordinate.xAixs.boundary[0].value"></el-input>
        <el-input style="width:100px" v-show="coordinate.xAixs.edit"
          v-model="coordinate.xAixs.boundary[1].value"></el-input>
        <el-icon :size="20" v-show="!coordinate.xAixs.edit" @click="coordinate.xAixs.edit = !coordinate.xAixs.edit"
          style="vertical-align: -5px;">
          <Edit />
        </el-icon>
        <el-icon :size="20" v-show="coordinate.xAixs.edit" @click="setAxis('x')" style="vertical-align: -5px;">
          <Share />
        </el-icon>
      </span>

      <span style="margin-left:10px;width:200px;">
        yAixs:&nbsp; <span v-show="!coordinate.yAixs.edit" style="width:200px">({{ coordinate.yAixs.boundary[0].value
        }},{{ coordinate.yAixs.boundary[1].value }})</span>
        <el-input style="width:100px" v-show="coordinate.yAixs.edit"
          v-model="coordinate.yAixs.boundary[0].value"></el-input>
        <el-input style="width:100px" v-show="coordinate.yAixs.edit"
          v-model="coordinate.yAixs.boundary[1].value"></el-input>

        <el-icon :size="20" v-show="!coordinate.yAixs.edit" @click="coordinate.yAixs.edit = !coordinate.yAixs.edit"
          style="vertical-align: -5px;">
          <Edit />
        </el-icon>
        <el-icon :size="20" v-show="coordinate.yAixs.edit" @click="setAxis('y')" style="vertical-align: -5px;">
          <Share />
        </el-icon>
      </span>

      <span style="margin-left:10px;width:200px;">
        最大次方:&nbsp; <span v-show="!editSquare" style="width:200px">{{ maxSquare }}</span>
        <el-input style="width:100px" v-show="editSquare" v-model="maxSquare"></el-input>
        <el-icon :size="20" v-show="!editSquare" @click="editSquare = !editSquare" style="vertical-align: -5px;">
          <Edit />
        </el-icon>
        <el-icon :size="20" v-show="editSquare" @click="editSquare = !editSquare" style="vertical-align: -5px;">
          <Share />
        </el-icon>
      </span>

    </div>

    <div style="position: relative;height:30%">
      <div class="LMsize"></div>
      <result style="width:100%;height:100%" :data="tableDate">
        <div>{{ funcStr }}</div>
      </result>
    </div>

  </div>
</template>

<script setup>
import * as echarts from 'echarts';
import * as global from '../../function/index'
import * as math from 'mathjs'
import { ref, reactive, onMounted } from 'vue'
import axios from 'axios'
import matApi from '../../api/BasicMatrixManage'
import { useStore } from 'vuex'
const store = useStore()

function updateEcharts() {
  setTimeout(function () {
    // Add shadow circles (which is not visible) to enable drag.
    myChart.setOption({
      graphic: data.map(function (item, dataIndex) {
        return {
          type: 'circle',
          position: myChart.convertToPixel('grid', item),
          shape: {
            cx: 0,
            cy: 0,
            r: symbolSize / 2
          },
          invisible: true,
          draggable: true,
          ondrag: function (dx, dy) {
            onPointDragging(dataIndex, [this.x, this.y]);
          },
          onmousemove: function () {
            showTooltip(dataIndex);
          },
          onmouseout: function () {
            hideTooltip(dataIndex);
          },
          z: 100
        };
      })
    });
  }, 1);
}

function setAxis(type) {
  if (type == "x") {
    myChart.setOption({
      xAxis: {
        min: xAixsMin.value * 1,
        max: xAixsMax.value * 1,
        type: 'value',
      },
      series: [
        {
          id: 'a',
          data: data
        }
      ],
    });
    coordinate.xAixs.edit = !coordinate.xAixs.edit
  }
  if (type == "y") {
    myChart.setOption({
      yAxis: {
        min: yAixsMin.value * 1,
        max: yAixsMax.value * 1,
        type: 'value',
      },
      series: [
        {
          id: 'a',
          data: data
        }
      ],
    });
    coordinate.yAixs.edit = !coordinate.yAixs.edit
  }

  updateEcharts();

}

const mat = ref(null)
onMounted(() => {
  document.getElementsByClassName('LMsize')[0].addEventListener('mousedown', function (e) { global.bindMouseDown(e, mat.value) })
})

//标记坐标轴的最小值和最大值，剔出超过这个界限的坐标点，被剔出的数据被视为删除
var xAixsMax = ref(70)
var xAixsMin = ref(-100)

var yAixsMax = ref(60)
var yAixsMin = ref(-30)

var maxSquare = ref(1) //x的最高次方
var editSquare = ref(false)
var weight = ref([])
var funcStr = ref("")
//最低次方暂时为1

var coordinate = reactive(
  {
    yAixs: {
      eidt: false,
      boundary: [yAixsMin, yAixsMax]
    },
    xAixs: {
      eidt: false,
      boundary: [xAixsMin, xAixsMax]
    }
  }
)


function addScatter() {
  data.push([math.random() * 20 - 10, math.random() * 20 - 10])
  myChart.setOption({
    series: [
      {
        id: 'a',
        type: 'scatter',
        smooth: true,
        symbolSize: symbolSize,
        data: data
      }
    ]
  })
  updateEcharts()
}

function mat_Request() {
  //在提交函数中处理越界的点，同时准备要上传接口的数据
  var data_temp = data
  data_temp = data_temp.filter(item => item[0] >= xAixsMin.value && item[0] <= xAixsMax.value)
  data = data_temp
  myChart.setOption({
    series: [
      {
        id: 'a',
        type: 'scatter',
        smooth: true,
        symbolSize: symbolSize,
        data: data
      }
    ]
  })
  updateEcharts()

  //开始处理给接口的参数  X*权值+c = Y  其中权值由最小二乘法得出  
  var X = []; var Y = [];
  data_temp.sort(function (a, b) { return a[0] - b[0] })
  for (var idx = 0; idx < data_temp.length; idx++) {
    X.push([]);
    for (var ii = 0; ii <= maxSquare.value; ii++) {
      X[idx].push(math.pow(data_temp[idx][0], ii));  //获得该数据的0次方到最高次方数据
    }
    Y.push([data_temp[idx][1]])
  }

  //获得XB = Y的最小二乘解需要同时在前面乘上X的转置矩阵，使其正交
  //先求x转置乘X的值
  var linearStr = ""  //用于最小二乘法计算的参数
  var str = "" + X[0].length + "|" + X.length + "|"  //col|row|
  for (var m_col = 0; m_col < X[0].length; m_col++) {
    for (var m_row = 0; m_row < X.length; m_row++) {
      str += X[m_row][m_col] + "|"
    }
  }
  str += X.length + "|" + X[0].length + "|"
  for (var m_row = 0; m_row < X.length; m_row++) {
    for (var m_col = 0; m_col < X[0].length; m_col++) {
      str += X[m_row][m_col] + "|"
    }
  }
  axios({
    method: 'get',
    url: '/api/?interface=matrix&intername=matrix_multip&value=' + str
  }).then(res => {
    var resultstr = global.xmlToValue(res.data);
    for (var idx = 0; idx < resultstr.length; idx++) {
      linearStr += resultstr[idx] + "|";
    }
  })

  //再求x转置乘y的值
  var str = "" + X[0].length + "|" + X.length + "|"  //col|row|
  for (var m_col = 0; m_col < X[0].length; m_col++) {
    for (var m_row = 0; m_row < X.length; m_row++) {
      str += X[m_row][m_col] + "|"
    }
  }
  str += Y.length + "|" + Y[0].length + "|"
  for (var m_row = 0; m_row < Y.length; m_row++) {
    for (var m_col = 0; m_col < Y[0].length; m_col++) {
      str += Y[m_row][m_col] + "|"
    }
  }
  axios({
    method: 'get',
    url: '/api/?interface=matrix&intername=matrix_multip&value=' + str
  }).then(res => {
    var resultstr = global.xmlToValue(res.data);
    for (var idx = 0; idx < resultstr.length; idx++) {
      linearStr += resultstr[idx] + "|";
    }
  })

  setTimeout(function () {
    axios({
      method: 'get',
      url: '/api/?interface=matrix&intername=linearRelation&value=' + linearStr
    }).then(res => {
      var resultstr = global.xmlToValue(res.data);
      weight.value = []
      funcStr.value = ""
      for (var idx = 2; idx < resultstr.length; idx++) { weight.value.push(resultstr[idx]) };
      funcStr.value = "Y=" + weight.value[0] + " + " + weight.value[1] + "x";

      if (weight.value.length > 2) {
        for (var idx = 2; idx < weight.value.length; idx++) {
          if (math.abs(weight.value[idx]) == 0) continue;
          funcStr.value += " + " + weight.value[idx] + "x^" + idx;
        }
      }
    })
  }, 50);


}


var edit = ref(false)
var description = ref("你好！")

var myChart
var option;
const symbolSize = 20;
var data = [
  [40, -10],
  [-30, -5],
  [-76.5, 20],
  [-63.5, 40],
  [-22.1, 50]
]

option = {
  title: {
    text: 'Try Dragging these Points',
    left: 'center'
  },
  tooltip: {
    triggerOn: 'none',
    formatter: function (params) {
      return (
        'X: ' +
        params.data[0].toFixed(2) +
        '<br>Y: ' +
        params.data[1].toFixed(2)
      );
    }
  },
  grid: {
    top: '8%',
    bottom: '12%'
  },
  xAxis: {
    min: -100,
    max: 70,
    type: 'value',
  },
  yAxis: {
    min: -30,
    max: 60,
    type: 'value',

  },
  dataZoom: [
    {
      type: 'slider',
      xAxisIndex: 0,
      filterMode: 'none'
    },
    {
      type: 'slider',
      yAxisIndex: 0,
      filterMode: 'none'
    },
    {
      type: 'inside',
      xAxisIndex: 0,
      filterMode: 'none'
    },
    {
      type: 'inside',
      yAxisIndex: 0,
      filterMode: 'none'
    }
  ],
  series: [
    {
      id: 'a',
      type: 'scatter',
      smooth: true,
      symbolSize: symbolSize,
      data: data
    }
  ]
};


onMounted(() => {
  myChart = echarts.init(document.querySelector("#demo"))
  updateEcharts()

  window.addEventListener('resize', updatePosition);
  myChart.on('dataZoom', updatePosition);

  option && myChart.setOption(option);
})

function updatePosition() {
  myChart.setOption({
    graphic: data.map(function (item, dataIndex) {
      return {
        position: myChart.convertToPixel('grid', item)
      };
    })
  });
}
function showTooltip(dataIndex) {
  myChart.dispatchAction({
    type: 'showTip',
    seriesIndex: 0,
    dataIndex: dataIndex
  });
}
function hideTooltip(dataIndex) {
  myChart.dispatchAction({
    type: 'hideTip'
  });
}
function onPointDragging(dataIndex, pos) {
  data[dataIndex] = myChart.convertFromPixel('grid', pos);
  // Update data
  myChart.setOption({
    series: [
      {
        id: 'a',
        data: data
      }
    ]
  });
}

const tableDate = reactive({
  return_m: [
    {
      matrix: [],
      row: 3,
      col: 3,
      result: true
    },
  ]
})


const activeName = ref('first')
</script>

<style scoped>
.matrixbox {
  height: 300px;
}

.matrix {
  border: 0;
  /*float: left;*/
}

.symbol {
  height: 50px;
  line-height: 50px;
  display: inline;
}

.LMsize {
  position: absolute;
  top: 0;
  width: 100%;
  height: 3px;
  cursor: col-resize;
  background-color: #d6d6d6;
}

.getans {
  right: 10px;
  top: -10px;
  position: absolute;
}

.el-icon-edit {
  height: 20px;
  width: 20px;
  background-color: orange;
}

.el-icon-check {
  height: 20px;
  width: 20px;
  background-color: gray;

}
</style>