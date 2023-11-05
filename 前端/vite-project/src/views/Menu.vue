<template>
  <div>
    <!--   <el-radio-group v-model="isCollapse" style="border:none;margin-bottom: 20px;">
    <el-radio-button v-if="isCollapse" :label="false" style="line-height=10px;"><el-icon><Expand /></el-icon>xx系统</el-radio-button>
    <el-radio-button v-else :label="true"  style="line-height=10px;"><el-icon><Fold /></el-icon>xx系统</el-radio-button>
  </el-radio-group>-->
    <div style="margin-bottom: 20px;">
      <el-icon v-if="isCollapse" @click="change">
        <Expand />
      </el-icon>
      <el-icon v-else @click="change">
        <Fold />
      </el-icon>
      xx系统
    </div>
    <el-menu :default-active="route.path" class="el-menu-vertical-demo" :collapse="isCollapse" router>
      <el-menu-item index="/"><template #title>
        <el-icon><Menu /></el-icon>
          <span>系统首页</span></template>
      </el-menu-item>
      <el-menu-item index="/basis"><template #title>
          <el-icon>
            <Briefcase />
          </el-icon>
          <span>基础工具</span></template>
      </el-menu-item>

      <el-sub-menu index="/dyn">
        <template #title>
          <span>离散动力系统</span>
        </template>
        <el-menu-item-group>
          <el-menu-item index="/dyn/dyn">离散动力系统</el-menu-item>
          <el-menu-item index="/dyn/diff">微分方程中的应用</el-menu-item>
          <el-menu-item index="/dyn/example">帮助页面</el-menu-item>
        </el-menu-item-group>
      </el-sub-menu>

      <el-sub-menu index="/orthogonality">
        <template #title>
          <span>线性模型</span>
        </template>
        <el-menu-item-group>
          <el-menu-item index="/orthogonality/linearmodel">曲线拟合</el-menu-item>
        </el-menu-item-group>
      </el-sub-menu>

      <el-sub-menu index="/save">
        <template #title>
          <span>保存内容</span>
        </template>
        <el-menu-item-group>
          <el-menu-item index="/save/history">保存内容</el-menu-item>
        </el-menu-item-group>
      </el-sub-menu>

    </el-menu>
  </div>
</template>

<script>
import { ref } from 'vue';
import { useRoute } from 'vue-router'
import { mapState } from 'vuex'

export default {
  setup() {
    //const isCollapse=ref(false)
    const route = useRoute()


    return {
      // isCollapse,
      route
    }
  },
  computed: {
    ...mapState(["isCollapse"])
  },
  data() {
    return {
    }
  },
  methods: {
    change() {
      this.$store.commit("changeIsCollapse")
    }
  }
}
</script>

<style>
.el-menu-vertical-demo:not(.el-menu--collapse) {
  width: 200px;
  min-height: 400px;
}
</style>