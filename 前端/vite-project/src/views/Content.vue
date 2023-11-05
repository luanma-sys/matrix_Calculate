<template>
  <div>

    <el-tabs  v-model="editableTabsValue" type="card" class="demo-tabs" editable @tab-remove="removeTab"
            @tab-add="addTab">
           
            <el-tab-pane v-for="item in editableTabs[0]" :key="item.name" :label="item.title" :name="item.name" >
              <router-view ></router-view>
           
            </el-tab-pane>

    </el-tabs>  




    
  </div>
</template>

<script lang="ts" setup>
//<result style="width: 100%" :data="store.state.tableDate"></result>
import { ref,reactive,onMounted,computed,watch } from 'vue'


import { useStore,mapState } from 'vuex'
const store = useStore()
const storeStateFns = mapState(["tableDate"])

const name = ref('aa')




const tableDate = reactive({

  return_m:[
    {
      matrix:[[1],[1]],
      row:3,
      col:3,
      result:true
    },
  ]
})



let tabIndex = 1
const editableTabsValue = ref('1')
const editableTabs = ref([
  [
    {
      title: 'Tab 1',
      name: '1',
      content: 'Tab 1 content\n\n\n\n1',
    },

  ],
])

const addTab = (targetName: string) => {
  const newTabName = `${++tabIndex}`
  editableTabs.value[0].push({
    title: 'New Tab',
    name: newTabName,
    content: 'New Tab content',
  })
  editableTabsValue.value = newTabName
}

const removeTab = (targetName: string) => {
  const tabs = editableTabs.value[0]
  let activeName = editableTabsValue.value
  if (activeName === targetName) {
    tabs.forEach((tab, index) => {
      if (tab.name === targetName) {
        const nextTab = tabs[index + 1] || tabs[index - 1]
        if (nextTab) {
          activeName = nextTab.name
        }
      }
    })
  }

  editableTabsValue.value = activeName
  editableTabs.value[0] = tabs.filter((tab) => tab.name !== targetName)
}

</script>

<style>
.head{
  height: 50px;
  background: rgb(248, 248, 248);
}

</style>