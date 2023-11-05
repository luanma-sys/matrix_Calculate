<template>
  <div style="height:100vh">
    <div ref="mat" class="matrixbox contentmat">
      <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
      </matrix>

      <div v-for="(add_matrix, index) in tableDate.add_matrix" :key="index" class="contentmat_item">
        <p style="float:left">+</p>
        <matrix :data="add_matrix" class="matrix" style="float:left"></matrix>
      </div>
      &nbsp;<el-icon @click="addMat" style="border:1px solid;">
        <Plus />
      </el-icon>
      <el-icon @click="reduceMat" style="border:1px solid;margin-top:90px;margin-left:-17px">
        <Minus />
      </el-icon>
    </div>

    <div>
      <el-button @click="mat_Request">计算结果</el-button>
    </div>
    <div style="position: relative;height:50%">
      <div class="btsize"></div>
      <result style="width:100%;height:100%" :data="tableDate"></result>
    </div>
  </div>
</template>

<script  setup>

import * as global from '../../function/index'
import { ref, reactive, onMounted } from 'vue'
import matApi from '../../api/BasicMatrixManage'
import { useStore } from 'vuex'
const store = useStore()
const mat = ref(null)
onMounted(() => {
  document.getElementsByClassName('btsize')[0].addEventListener('mousedown', function (e) { global.bindMouseDown(e, mat.value) })
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
  add_matrix: [
    {
      row: 3,
      col: 3,
      matrix: [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
      result: false
    },
    {
      row: 3,
      col: 3,
      matrix: [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
      result: false
    },
  ],
  return_m: [
    {
      matrix: [],
      row: 3,
      col: 3,
      result: true
    },
  ]
})


function mat_Request() {

  // console.log("tab  "+tableDate.matrix[0].row)
  //拼接网络请求需要的参数
  var str = "" + tableDate.matrix[0].row + "|" + tableDate.matrix[0].col + "|";
  for (let row = 0; row < tableDate.matrix[0].row; row++) {
    for (let col = 0; col < tableDate.matrix[0].col; col++) {
      str = str + tableDate.matrix[0].matrix[row][col] + "|";
    }
  }

  for (let idx = 0; idx < tableDate.add_matrix.length; idx++) {
    str += tableDate.matrix[0].row + "|" + tableDate.matrix[0].col + "|";
    for (let row = 0; row < tableDate.matrix[0].row; row++) {
      for (let col = 0; col < tableDate.matrix[0].col; col++) {
        str = str + tableDate.add_matrix[idx].matrix[row][col] + "|";
      }
    }
  }

  //发起网络请求
  matApi.getMatAdd(str).then(res => {
    console.log(res.data)
    var resultstr = global.xmlToValue(res.data);
    tableDate.return_m[0].matrix = [];
    for (var row = 0; row < resultstr[0]; row++) {
      var temp = []
      for (var col = 0; col < resultstr[1]; col++) {
        temp.push(resultstr[2 + row * resultstr[0] + col]);
      }

      tableDate.return_m[0].matrix.push(temp);
    }
  })



  //store.commit

  // 
}



function addMat() {
  tableDate.add_matrix.push({
    row: 3,
    col: 3,
    matrix: [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
    result: false
  })
}
function reduceMat() {
  if (tableDate.add_matrix.length <= 1) {
    alert("已经无法减少矩阵数量");
    return;
  }

  tableDate.add_matrix.pop();
}

</script>

<style scoped>
/*.demo-tabs > .el-tabs__content {
  padding: 32px;
  color: #6b778c;
  font-size: 32px;
  font-weight: 600;
}
.demo-tabs{
  height: 60px;
}*/
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

.btsize {
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
</style>