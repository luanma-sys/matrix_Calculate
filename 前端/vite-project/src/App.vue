<template>
  <div>
    <div class="menu">
      <Menu></Menu>
    </div>
    <div class="context" :class="{ active: $store.state.isCollapse }">
      <div v-show="$store.state.matMessageSuspend" ref="showPanel" class="matMessage">
        row:<el-input v-model="mRow"  class="w-50 w" placeholder="Please input" :value="mRow" />
        col:<el-input v-model="mCol"  class="w-50 w" placeholder="Please input" :value="mCol" />
        <button @click="submitmat" style="float:right" class="matMessage_item" >确定</button>
      </div> <!--这里使用ref(matrix里的defineProps)对象会报错-->
      <Content></Content>
    </div>

  </div>
</template>

<script setup>
import Menu from './views/Menu.vue'
import Content from './views/Content.vue'
import { ref, onMounted, computed, watch } from 'vue'
import { useStore } from 'vuex'

const showPanel = ref(null)
const panel = ref(null);
const store = useStore()
const input = ref('aa')


var mRow = computed({
  get: () => {
    return store.state.row
  },
  set: (v) => {
    store.commit("setRow", v)
  }
})


var mCol = computed({
  get: () => {
    return store.state.col
  },
  set: (v) => {
    store.commit("setCol", v)
  }
})
/*
watch(mRow, (value, oldValue) => {
  console.log("mR")
  console.log(mRow.value)
})*/
function submitmat() {
  store.commit("submitmat")
}



function close() {
  store.commit("closeMatMessageSuspend")
}


onMounted(() => {
  document.addEventListener('click', (e) => {
    if (showPanel.value) {
      let isSelf = showPanel.value.contains(e.target)
      //store.commit("setRow")
      if (!isSelf && store.state.matMessageSuspend) {  //在点击的不是自己 且 自己打开时执行关闭函数

        close()
      }
    }
  })
});


/* 这个函数的作用是调用open函数，现在改到matrix组件里实现
onMounted(()=>{
   document.addEventListener('click',(e)=>{
     console.log(panel.value)
    if(panel.value){
     console.log(panel.value)
       let isSelf = panel.value.contains(e.target)
        if(isSelf){
         console.log("matrix in")
         //open()
        }
    }
})
 });*/

</script>

<style>
.menu {
  position: fixed;
  left: 0;
  top: 0;
  bottom: 0;
}

.context {
  padding-left: 200px;
}

.active {
  padding-left: 64px;

}

.matMessage {
  position: absolute;
  top: 50px;
  /*background-color: #eeeef2;*/
  background: -webkit-linear-gradient(left, rgba(253, 253, 253, 1)0%,rgba(250, 250, 250, 0.9)60%, rgba(245, 245, 245, 0.4)99%);
  background:linear-gradient(left,rgba(0,0,0,1)0%,rgba(0,0,0,0.5)66%, rgba(0,0,0,0)99%);
  height: 52px;
  width: 90%;
  z-index: 10;
}
.matMessage_item {
  margin-right: 100px;
}

.w {
  max-width: 200px;
  /*这里使用width会被其他css的width覆盖*/
  margin-top: 13px;
  height: 30px;
}</style>