<template>
  <div>
    data
  </div>
</template>

<script setup>
import sqlApi from '../../api/MatrixSql'
import { useStore } from 'vuex'
  sqlApi.getMat('test').then(res=>{
    console.log(res)
  })
</script>

<style>

</style>