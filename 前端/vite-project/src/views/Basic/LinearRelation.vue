<template>
    <div style="height:100vh">
      <div ref="mat" class="matrixbox contentmat">
      <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
      </matrix>

      <div v-for="(mul_matrix,index) in tableDate.mul_matrix" :key="index"  class="contentmat_item">
        <p style="float:left">&nbsp;&nbsp;&nbsp;结果:</p><matrix :data="mul_matrix" class="matrix" style="float:left"></matrix>
      </div>
    </div>

         <div > 
            <el-button @click="mat_Request">计算结果</el-button>
         </div>
      <div style="position: relative;height:50%" >
         <div class="linesize" ></div>
         <result style="width:100%;height:100%" :data="tableDate"></result>
      </div>

    </div>
</template>

<script  setup>
import matrix from '../../components/Matrix.vue'
import matApi from '../../api/BasicMatrixManage'
import {ref,reactive, onMounted} from 'vue'
import * as global from '../../function/index'

const mat = ref(null)
onMounted(()=>{
  
  document.getElementsByClassName('linesize')[0].addEventListener('mousedown', function(e)
    {      global.bindMouseDown(e,mat.value) }    )  

})
  

const tableDate = reactive({
  matrix:[
    {
      id:1,
      row:3,
      col:3,
      matrix:[[1,2,3],[3,4,5],[5,6,7]],
      result: false
    }
  ],
  mul_matrix:[
    {
      row:3,
      col:1,
      matrix:[[1],[0],[0]],
      result:false
    },
  ],
  return_m:[
    {
      matrix:[[1,1,1],[1,1,1],[1,1,1]],
      row:3,
      col:3,
      result:true
    },
  ]
})
function mat_Request(){
  if(tableDate.matrix[0].col != tableDate.mul_matrix[0].row)  { alert("矩阵与结果矩阵不匹配"); return; }

    //拼接网络请求需要的参数
  var str = ""+tableDate.matrix[0].row+"|"+tableDate.matrix[0].col+"|";
  for(let row=0;row<tableDate.matrix[0].row;row++)
  {
    for(let col=0;col<tableDate.matrix[0].col;col++)
    {
      str = str+tableDate.matrix[0].matrix[row][col]+"|";
    }
  }

  str +=  tableDate.mul_matrix[0].row+"|"+tableDate.mul_matrix[0].col+"|";
  for(let row=0;row<tableDate.mul_matrix[0].row;row++)
  {
    for(let col=0;col<tableDate.mul_matrix[0].col;col++)
    {
      str = str+tableDate.mul_matrix[0].matrix[row][col]+"|";
    }
  }
  //发起网络请求

  matApi.getLinearRelation(str).then(res=>{
    var resultstr =  global.xmlToValue(res.data);

    tableDate.return_m[0].matrix=[];
    for(var row=0;row<resultstr[0];row++)
    {
      var temp = []
      for(var col=0;col<resultstr[1];col++)
      {
        temp.push(resultstr[2+row]);
      }
      tableDate.return_m[0].matrix.push(temp);
    }
  })
}

</script>

<style>

.matrixbox{
  height: 300px;
}
.matrix{
    border:0;
  /*float: left;*/
}
.symbol{
  height: 50px;
  line-height: 50px;
  display: inline;
}

.linesize {
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