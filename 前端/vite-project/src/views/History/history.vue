<template>
    <div>
        <el-table :data="tableData" style="width: 100%">
            <el-table-column fixed prop="matname" label="Matname" width="100" />
            <el-table-column fixed prop="mattype" label="Mattype" width="100" />
            <el-table-column prop="matmajor" label="Matmajor" width="250" />
            <el-table-column prop="matminor" label="Matminor" width="250" />
            <el-table-column prop="matresult" label="Matresult" width="250" />
            <el-table-column fixed="right" label="Operations" width="120">
            <template #default="scope">
                <!--<el-button link type="primary" size="small">Edit</el-button>-->
                <router-link :to="{
                  path: '/dyn/dyn',
                  query:{
                    matname: scope.row.matname,
                    mattype: scope.row.mattype,
                    matmajor: scope.row.matmajor,
                    matminor: scope.row.matminor,
                    matresult: scope.row.matresult,
                    isStorage: true
                  }
                }">加载数据</router-link>
            </template>
            </el-table-column>
        </el-table>
    </div>
</template>
  
<script setup>
import sqlApi from '../../api/MatrixSql'
import { useStore } from 'vuex'
import * as global from '../../function/index'
import {ref,reactive,onMounted,onBeforeMount} from 'vue'

const tableData=ref([])

onBeforeMount(()=>{
    sqlApi.getAllMat().then(res=>{
        var resultstr = global.xmlToMat(res.data)
        
        for(var i=0;i<resultstr.data.matname.length;i++){
          tableData.value.push({
            matname: resultstr.data.matname[i],
            mattype: resultstr.data.mattype[i],
            matmajor: decodeURI(resultstr.data.matmajor[i]),
            matminor: decodeURIComponent(resultstr.data.matminor[i]),
            matresult: resultstr.data.matresult[i]
          })
        }
    })
})

const handleClick = () => {
  console.log('click')
}



</script>
  
<style>
  
</style>