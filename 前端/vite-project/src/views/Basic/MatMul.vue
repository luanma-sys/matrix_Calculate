<template>
    <div style="height:100vh">
      

      <div ref="mat"  class="matrixbox contentmat">
      <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
      </matrix>

            <div v-for="(mul_matrix, index) in tableDate.mul_matrix" :key="index" class="contentmat_item">
                <p style="float:left">*</p>
                <matrix :data="mul_matrix" class="matrix" style="float:left"></matrix>
            </div>
            &nbsp;<el-icon @click="addMat" style="border:1px solid;"><Plus /></el-icon>
            <el-icon @click="reduceMat" style="border:1px solid;margin-top:90px;margin-left:-17px"><Minus /></el-icon>
        </div>

        <div > 
            <el-button @click="mat_Request">计算结果</el-button>
         </div>
         
      <div style="position: relative;height:50%" >
         <div class="mulsize" ></div>
         <result style="width:100%;height:100%" :data="tableDate"></result>
      </div>

    </div>
</template>

<script  setup>
import { ref, reactive, onMounted } from 'vue'

import matApi from '../../api/BasicMatrixManage'
import * as global from '../../function/index'

const mat = ref(null)
onMounted(()=>{
  document.getElementsByClassName('mulsize')[0].addEventListener('mousedown', function(e)
    {      global.bindMouseDown(e,mat.value) }    )  

})

const tableDate = reactive({
    matrix: [
        {
            id: 1,
            row: 3,
            col: 3,
            matrix: [[1, 2, 3], [3, 4, 5], [5, 6, 7]],
            result: false
        }
    ],
    mul_matrix: [
        {
            matrix: [[1, 0, 0], [0, 1, 0], [0, 0, 1]],
            row: 3,
            col: 3,
            result: false
        },
        {
            matrix: [[1, 0, 0], [0, 1, 0], [0, 0, 1]],
            row: 3,
            col: 3,
            result: false
        },
    ],
    return_m: [
        {
            matrix: [[1, 0, 0], [0, 1, 0], [0, 0, 1]],
            row: 3,
            col: 3,
            result: true
        },

    ]
})
function mat_Request() {
    //拼接网络请求需要的参数
    var str = "" + tableDate.matrix[0].row + "|" + tableDate.matrix[0].col + "|";
    for (let row = 0; row < tableDate.matrix[0].row; row++) {
        for (let col = 0; col < tableDate.matrix[0].col; col++) {
            str = str + tableDate.matrix[0].matrix[row][col] + "|";
        }
    }

for(let idx=0;idx<tableDate.mul_matrix.length;idx++)
{
    str += tableDate.matrix[0].row + "|" + tableDate.matrix[0].col + "|";
    for (let row = 0; row < tableDate.matrix[0].row; row++) {
        for (let col = 0; col < tableDate.matrix[0].col; col++) {
            str = str + tableDate.mul_matrix[idx].matrix[row][col] + "|";
        }
    }
}
    //发起网络请求
    matApi.getMatMultip(str).then(res => {
        var resultstr = global.xmlToValue(res.data);
        if (resultstr.indexOf("failed") !== -1) {
            alert("矩阵信息有误！");
            return
        }

     
        tableDate.return_m[0].matrix = [];
        for (var row = 0; row < resultstr[0]; row++) {
            var temp = []
            for (var col = 0; col < resultstr[1]; col++) {
                temp.push(resultstr[2 + row * resultstr[0] + col]);
            }
            tableDate.return_m[0].matrix.push(temp);
        }
    })
}


function addMat()
{
  tableDate.mul_matrix.push({
      row:3,
      col:3,
      matrix:[[0,0,0],[0,0,0],[0,0,0]],
      result:false
    })
}
function reduceMat()
{
  if(tableDate.mul_matrix.length<=1)
  {
    alert("已经无法减少矩阵数量");
    return;
  }

  tableDate.mul_matrix.pop();
}

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

.mulsize {
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