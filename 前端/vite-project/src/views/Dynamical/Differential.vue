<template>
  <div style="height:100vh">
    <el-tabs v-model="activeName" class="demo-tabs">
      <el-tab-pane label="微分方程中的应用" name="first"></el-tab-pane>
    </el-tabs>

      <div ref="mat" class="matrixbox contentmat">
          <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
          </matrix>

          <div v-for="(mul_matrix, index) in tableDate.basic_matrix" :key="index" class="contentmat_item">
              <p style="float:left" class="symbol">初始状态:</p>
              <matrix :data="mul_matrix" class="matrix" style="float:left"></matrix>
          </div>
      </div>

         <div > 
            <el-button @click="mat_Request">计算结果</el-button>
         </div>
      <div style="position: relative;height:50%" >
         <div class="diffsize" ></div>
         <result style="width:100%;height:100%" :data="tableDate">
          <div id="demo" style="width:100%;height:400px"></div>
         </result>
      </div>

  </div>
</template>

<script setup>
import { ref, reactive, onMounted } from 'vue'
import axios from 'axios'
import matrix from '../../components/Matrix.vue'
import * as global from '../../function/index'
import * as math from 'mathjs'
import * as echarts from 'echarts';
import matApi from '../../api/BasicMatrixManage'

const mat = ref(null)
onMounted(()=>{
  document.getElementsByClassName('diffsize')[0].addEventListener('mousedown', function(e)
    {      global.bindMouseDown(e,mat.value) }    )  
})

var option
var myChart
var iternum = ref(0)  //特征值迭代次数
//    var eigen_temp = str;  //将str保存起来，用于获得长期行为中的特征值,这个变量在axios请求中会用到

onMounted(() => {
  myChart = echarts.init(document.querySelector("#demo"))
  option = {
  };
  myChart.setOption(option);
})


const tableDate = reactive({
  matrix: [
      {
          id: 1,
          row: 3,
          col: 3,
          matrix: [[0, 0, 0.33], [0.18, 0, 0], [0, 0.71, 0.94]],
          result:false
      }
  ],
  basic_matrix: [
      {
          row:3,
          col:1,
          matrix: [[1], [0], [0]],
          result:false
      },
  ],
  return_m:[
    {
//         row:resultstr[0],
//         col:"1",
//         matrix:[],
//         eigen:null,
//         constant:null,
//         partial:1,     //偏导值，当为常数时不显示
//         result:true
      }
  ]
})

function mat_Request(){
  if(tableDate.matrix[0].col != tableDate.basic_matrix[0].row)  { alert("矩阵与初始状态不匹配"); return; }


  //拼接网络请求需要的参数
var str = ""+tableDate.matrix[0].row+"|"+tableDate.matrix[0].col+"|";
for(let row=0;row<tableDate.matrix[0].row;row++)
{
  for(let col=0;col<tableDate.matrix[0].col;col++)
  {
    str = str+tableDate.matrix[0].matrix[row][col]+"|";
  }
}


//发起求特征向量网络请求
axios({
  method:'get',
  url:'/api/?interface=matrix&intername=matrix_rank_vectors&value='+str
}).then(res=>{
  var resultstr =  global.xmlToValue(res.data);
  tableDate.return_m=[]; 
  //先按数组的列遍历，在案数组的行遍历，把每一列单独拿出来，做成每行只有一个元素的二维数组
  for(var col=0;col<resultstr[1];col++)
  { //这里初始化结果数组
    tableDate.return_m.push({row:resultstr[0],col:"1",matrix:[],eigen:null,constant:null,result:true,partial:null});
    var matrix = []
    
    for(var row=0;row<resultstr[0];row++)
    {
      matrix.push([resultstr[2 + row + col * resultstr[1]]]);
    }
    
    tableDate.return_m[col].matrix=matrix;
  }
})

var eigen_temp = str;  //将str保存起来，用于获得长期行为中的特征值
setTimeout(function(){
//发起求特征值的网络请求
axios({
  method:'get',
  url:'/api/?interface=matrix&intername=matrix_rank&value='+str
}).then(res=>{
  var resultstr =  global.xmlToValue(res.data);
  //先按数组的列遍历，在案数组的行遍历，把每一列单独拿出来，做成每行只有一个元素的二维数组
  for(var i=0;i<resultstr.length;i++)
  {
    tableDate.return_m[i].eigen=resultstr[i];
    tableDate.return_m[i].partial="e^"+resultstr[i];
  }
})
},20) 
//发起求初始向量在特征向量上的各个值
str +=  tableDate.basic_matrix[0].row+"|"+tableDate.basic_matrix[0].col+"|";
for(let row=0;row<tableDate.basic_matrix[0].row;row++)
{
  for(let col=0;col<tableDate.basic_matrix[0].col;col++)
  {
    str = str+tableDate.basic_matrix[0].matrix[row][col]+"|";
  }
}


//发起线性解接口请求
setTimeout(function () {
    //判断是否可以进行接口请求

    //发起 求初始向量在特征向量上的各个值 //特征向量 * X = 初始状态
    str = tableDate.return_m[0].row + "|" + tableDate.return_m[0].row + "|";
    for (let idx = 0; idx < tableDate.return_m.length; idx++) {
      for (let row = 0; row < tableDate.return_m[0].row; row++) {

        str = str + tableDate.return_m[idx].matrix[row][0] + "|";
      }
    }
    str += tableDate.basic_matrix[0].row + "|" + tableDate.basic_matrix[0].col + "|";
    for (var row = 0; row < tableDate.basic_matrix[0].row; row++) {
      for (var col = 0; col < tableDate.basic_matrix[0].col; col++) {
        str += tableDate.basic_matrix[0].matrix[row][col] + "|";
      }
    }
    // console.log(str)
    axios({
      method: 'get',
      url: '/api/?interface=matrix&intername=linearRelation&value=' + str
    }).then(res => {
      var resultstr = global.xmlToValue(res.data); //这里接口返回的是一个矩阵，因为这里接收的是常数，所以要跳过前两个值，他们为row和col
      //先按数组的列遍历，在案数组的行遍历，把每一列单独拿出来，做成每行只有一个元素的二维数组
      for (var i = 2; i < resultstr.length; i++) {
        if(resultstr[i]!==NaN)
        tableDate.return_m[i - 2].constant = resultstr[i];
      }
    })

  }, 80)
  //设置echarts视图
  
  setTimeout(function () {


    option = {
  title: {
    text: 'Stacked Line'
  },
  tooltip: {
    trigger: 'axis'
  },
  legend: {
    data: ['Email', 'Union Ads', 'Video Ads', 'Direct', 'Search Engine']
  },
  grid: {
    left: '3%',
    right: '4%',
    bottom: '3%',
    containLabel: true
  },
  toolbox: {
    feature: {
      saveAsImage: {}
    }
  },
  xAxis: {
    type: 'category',
    boundaryGap: false,

  },
  yAxis: {
    type: 'value'
  },
  series: [
    {
      name: 'Email',
      type: 'line',
      data: [120, 132, 101, 134, 90, 230, 210]
    },
    {
      name: 'Union Ads',
      type: 'line',
      data: [220, 182, 191, 234, 290, 330, 310]
    },
    {
      name: 'Video Ads',
      type: 'line',
      data: [150, 232, 201, 154, 190, 330, 410]
    },
    {
      name: 'Direct',
      type: 'line',
      data: [320, 332, 301, 334, 390, 330, 320]
    },
    {
      name: 'Search Engine',
      type: 'line',
      data: [820, 932, 901, 934, 1290, 1330, 1320]
    }
  ]
};
    var data_temp = []
    for(var idx=0; idx<tableDate.return_m.length;idx++) { data_temp.push([]); }

    iternum = 5;
    for (var count = 1; count <= iternum; count++) {
        axios({
        method: 'get',
        url: '/api/?interface=matrix&intername=matrix_rank&value=' + eigen_temp + '&eigen_count=' + count
      }).then(res => {
        var resultstr = global.xmlToValue(res.data); //这里接口返回的是一个矩阵，因为这里接收的是常数，所以要跳过前两个值，他们为row和col

        for (var ii = 0; ii < tableDate.return_m[0].row; ii++) {//ii代表特征向量的row
          var count = 0
          for (var idx = 0; idx < tableDate.return_m.length; idx++) {//idx代表第几个特征向量
            var sign = 1;       //sign代表虚数的正负，指数虚数的表现形式为e^数字，如果数字为负则第三位为-号
            if(tableDate.return_m[idx].partial[2]==='-')  { sign = -1; }

            count += tableDate.return_m[idx].constant
              * math.abs(math.complex(resultstr[idx]))
              * tableDate.return_m[idx].matrix[ii][0]     //特征向量只有一列，所以第二个括号用0表示
              * math.pow(math.e,math.abs(math.complex(tableDate.return_m[idx].partial.substr(2))) *sign )
              //        自然底数  获取虚数模    获取虚数                             去掉虚数前的e^  保证符号不变  
              
          }
          data_temp[ii].push(count)
        }
        
      })
    }
    setTimeout(function(){

    option.series=[]
    option.legend.data = []
    for(var idx=1; idx<=tableDate.return_m.length; idx++)
    {
      option.legend.data.push('eigen'+idx)
      option.series.push({
          name: 'eigen'+idx,
          type: 'line',
         // data: [1,2,3]
          data: data_temp[idx-1]
    })
    }
    myChart.setOption(option);
    },40)

   
  }, 120)


}




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
  height: 150px;
  line-height: 80px;
  display: inline;
}

.diffsize {
position: absolute;
top: 0;
width: 100%;
height: 3px;
cursor: col-resize;
background-color: #d6d6d6;
}

.getans{
  right: 10px;
  top: -10px;
  position: absolute;
}
</style>