<template>
  <div style="height:100vh">
    <div ref="mat" class="matrixbox contentmat">
      <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
      </matrix>
    </div>

    <div> <el-button @click="mat_Request('area')">计算矩阵面积</el-button>
      <el-button @click="mat_Request('dimensions')">计算矩阵维数</el-button>
    </div>

    <div style="position: relative;height:50%">
      <div class="areasize"></div>
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
  
  document.getElementsByClassName('areasize')[0].addEventListener('mousedown', function(e)
    {      global.bindMouseDown(e,mat.value) }    )  

})

const tableDate = reactive({
  matrix: [
    {
      id: 1,
      row: 3,
      col: 3,
      matrix: [[1, 0, 0], [0, -3, 0], [0, 0, 0]],
      result: false
    }
  ],
  return_m: [
    {
      //    matrix:[[1,1,1],[1,1,1],[1,1,1]],
      //    row:3,
      //   col:3,
      result: true,
      areaOrDimensions: null,
    },
  ]
})
function mat_Request(type) {
  //拼接网络请求需要的参数
  var str = "" + tableDate.matrix[0].row + "|" + tableDate.matrix[0].col + "|";
  for (let row = 0; row < tableDate.matrix[0].row; row++) {
    for (let col = 0; col < tableDate.matrix[0].col; col++) {
      str = str + tableDate.matrix[0].matrix[row][col] + "|";
    }
  }

  if (type == "area") {
    matApi.getMatArea(str).then(res => {
      var resultstr = global.xmlToValue(res.data);
      tableDate.return_m[0].areaOrDimensions = resultstr[0];
    })
  }

  if (type == "dimensions") {
    matApi.getMatDimensions(str).then(res => {
      var resultstr = global.xmlToValue(res.data);
      tableDate.return_m[0].areaOrDimensions = resultstr[0];
    })
  }

}
</script>

<style scoped>
.matrixbox {
  height: 300px;
}

.matrix {
  border: 0;
}

.symbol {
  height: 50px;
  line-height: 50px;
  display: inline;
}

.areasize {
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