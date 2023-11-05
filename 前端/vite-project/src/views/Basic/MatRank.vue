<template>
  <div style="height:100vh">
    <div ref="mat" class="matrixbox contentmat">
      <matrix :data="tableDate.matrix[0]" class="matrix contentmat_item">
      </matrix>
    </div>

    <div> <el-button @click="mat_Request('rank')">计算特征值</el-button>
      <el-button @click="mat_Request('vectors')">计算特征向量</el-button>
      <el-button @click="mat_Request('all')">同时计算两者</el-button>
    </div>

    <div style="position: relative;height:50%">
      <div class="ranksize"></div>
      <result style="width:100%;height:100%" :data="tableDate"></result>
    </div>

  </div>
</template>

<script  setup>
import { ref, reactive, onMounted } from 'vue'
import matApi from '../../api/BasicMatrixManage'
import * as global from '../../function/index'

const mat = ref(null)
onMounted(() => {

  document.getElementsByClassName('ranksize')[0].addEventListener('mousedown', function (e) { global.bindMouseDown(e, mat.value) })

})

const tableDate = reactive({
  matrix: [
    {
      id: 1,
      row: 3,
      col: 3,
      matrix: [[0, 0, 0.33], [0.18, 0, 0], [0, 0.71, 0.94]],
      result: false
    }
  ],
  return_m: [
    {
      matrix: null,
      row: 3,
      col: 3,
      result: true,
      eigen_gather: [],
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
  tableDate.return_m[0].eigen_gather = null;
  tableDate.return_m[0].matrix = null;


  if (type == "vectors" || "all" == type) {
    //发起网络请求
    matApi.getMatRankVectors(str).then(res => {
      var resultstr = global.xmlToValue(res.data);

      //tableDate.return_m[0].matrix = [];
      tableDate.return_m.splice(1);
      //把返回结果拼成数组放入result里

      for (var col = 0; col < resultstr[1]; col++) {
        var temp = []
        for (var row = 0; row < resultstr[0]; row++) {

          temp.push([resultstr[2 + row + col * resultstr[1]]]);


        }
        if(col!=resultstr[1]-1) tableDate.return_m.push({ row: resultstr[0], col: "1", matrix: [], eigen: null, constant: null, result: true });
        tableDate.return_m[col].matrix = temp
      }

    })
  }
  if (type == "rank" || type == "all") {
    //发起网络请求
    matApi.getMatRank(str).then(res => {
      var resultstr = global.xmlToValue(res.data);
      tableDate.return_m[0].eigen_gather = resultstr;//把结果集放进eigen_gather 里
    })
  }

}

</script>

<style>
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

.ranksize {
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