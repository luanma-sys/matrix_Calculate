<template>
    <div style="height:100vh">
      <div ref="mat" class="matrixbox contentmat">
      <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
      </matrix>

    </div>

        <div > 
            <el-button @click="mat_Request">计算结果</el-button>
         </div>
      <div style="position: relative;height:50%" >
         <div class="transize" ></div>
         <result style="width:100%;height:100%" :data="tableDate"></result>
      </div>

    </div>
</template>

<script  setup>
import {ref,reactive, onMounted} from 'vue'
import matApi from '../../api/BasicMatrixManage'
import * as global from '../../function/index'

const mat = ref(null)
onMounted(()=>{
  document.getElementsByClassName('transize')[0].addEventListener('mousedown', function(e)
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
    //拼接网络请求需要的参数
  var str = ""+tableDate.matrix[0].row+"|"+tableDate.matrix[0].col+"|";
  for(let row=0;row<tableDate.matrix[0].row;row++)
  {
    for(let col=0;col<tableDate.matrix[0].col;col++)
    {
      str = str+tableDate.matrix[0].matrix[row][col]+"|";
    }
  }

  matApi.getMatTranspose(str).then(res=>{
    var resultstr =  global.xmlToValue(res.data);

    tableDate.return_m[0].matrix=[];
    //把返回结果拼成数组放入result里
    for(var row=0;row<resultstr[0];row++)
    {
      var temp = []
      for(var col=0;col<resultstr[1];col++)
      {
        temp.push(resultstr[2+row*resultstr[0]+col]);
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
}
.symbol{
  height: 50px;
  line-height: 50px;
  display: inline;
}

.transize {
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