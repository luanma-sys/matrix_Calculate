<template>
    <div>
      <el-tabs v-model="activeName" class="demo-tabs">
    <el-tab-pane label="矩阵加法" name="first"><MatAdd></MatAdd></el-tab-pane>
    <el-tab-pane label="矩阵乘法" name="second"><MatMul></MatMul></el-tab-pane>
    <el-tab-pane label="矩阵阶梯化" name="third"><MatEchelon></MatEchelon></el-tab-pane>
    <el-tab-pane label="矩阵转置" name="fourth"><MatTranspose></MatTranspose></el-tab-pane>
    <el-tab-pane label="矩阵大小/维数" name="fifth"><MatArea></MatArea></el-tab-pane>
<!--    <el-tab-pane label="矩阵逆" name="sixth">Task</el-tab-pane>   -->
    <el-tab-pane label="Ax=b" name="seventieth"><LinearRelation></LinearRelation></el-tab-pane>
    <el-tab-pane label="特征值" name="eighth"><MatRank></MatRank></el-tab-pane>
<!--    <el-tab-pane label="奇异值" name="ninth">Task</el-tab-pane>   -->
  </el-tabs>

    </div>
</template>

<script setup>
import matrix from '../components/Matrix.vue'


import {ref,reactive} from 'vue'
import axios from 'axios'
import x2js from 'x2js'

import * as global from '../function/index'


import { MatAdd,MatMul,MatEchelon,MatTranspose,MatArea,LinearRelation,MatRank } from './Basic'

/*let test = ref("a")

const tableDate = reactive({
  matrix:[
    {
      id:1,
      row:3,
      col:3,
      matrix:[[1,2,3],[3,4,5],[5,6,7]]
    }
  ],
  add_matrix:[
    {
      matrix:[[0,0,0],[0,0,0],[0,0,0]]
    },
    {
      matrix:[[0,0,0],[0,0,0],[0,0,0]]
    },
    {
      matrix:[[0,0,0],[0,0,0],[0,0,0]]
    },
    {
      matrix:[[0,0,0],[0,0,0],[0,0,0]]
    },
  ],
  return_m:[
    {
      matrix:[[1,1,1],[1,1,1],[1,1,1]],
      result:true
    },
  ]
})

function matrix_add(){
  var str = ""+tableDate.matrix[0].row+"|"+tableDate.matrix[0].col+"|";
  for(let row=0;row<tableDate.matrix[0].row;row++)
  {
    for(let col=0;col<tableDate.matrix[0].col;col++)
    {
      str = str+tableDate.matrix[0].matrix[row][col]+"|";
    }
  }

  str +=  tableDate.matrix[0].row+"|"+tableDate.matrix[0].col+"|";
  for(let row=0;row<tableDate.matrix[0].row;row++)
  {
    for(let col=0;col<tableDate.matrix[0].col;col++)
    {
      str = str+tableDate.add_matrix[0].matrix[row][col]+"|";
    }
  }

  axios({
    method:'get',
    url:'/api/?interface=matrix&intername=matrix_add&value='+str
  }).then(res=>{
    test.value =  global.xmlToValue(res.data);
    tableDate.return_m[0].matrix=[];
    for(var row=0;row<test.value[0];row++)
    {
      var temp = []
      for(var col=0;col<test.value[1];col++)
      {
        temp.push(test.value[2+row*test.value[0]+col]);
      }
      tableDate.return_m[0].matrix.push(temp);
    }
  })
}*/

const activeName = ref('first')


</script>

<style>
/*.demo-tabs > .el-tabs__content {
  padding: 32px;
  color: #6b778c;
  font-size: 32px;
  font-weight: 600;
}
.demo-tabs{
  height: 60px;
}*/



</style>