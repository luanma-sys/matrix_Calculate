<template>
  <div>
    <div class="indexhead">
      <h3>一个计算线性代数相关功能的系统</h3>
      <div>本系统是一个使用了c/c++、eigen、vue的线代工具系统，具有全面的线代基础运算工具，多样的线性代数模型，并使用了echarts为数据提供可视化处理
        ，且支持复数运算
      </div>
      <h3>特性</h3>
      <ul>
        <li>支持复数的矩阵运算</li>
        <li>全面的线代基础运算</li>
        <li>矩阵模型可视化</li>
        <li>易于扩展的服务器</li>
        <li>开源</li>
      </ul>
    </div>
    <div class="indexbody">
      <el-collapse style="margin-top:30px" v-model="activeName" accordion>
        <el-collapse-item title="开始使用" name="1">
          <div>
            基础工具:<router-link to='/basis'>首页</router-link>
          </div>
          <div>
            第一个样例:<router-link to='/dyn/example'>离散动力系统样例</router-link>
          </div>
        </el-collapse-item>

        <el-collapse-item title="仓库" name="3">
          <div>
            <a href="https://github.com/luanma-sys/-_-">https://github.com/luanma-sys/-_-</a>
          </div>
        </el-collapse-item>
        <el-collapse-item title="版本" name="4">
          <div>
            0.0.1 version
          </div>
        </el-collapse-item>
      </el-collapse>
    </div>
    <div class="indexfoot">
      <div>
        联系方式：
        <ul>
          <li>qq:123456</li>
          <li>微信:abcddd</li>
          <li>邮箱:1125932819@qq.com</li>
          <li>github:<a href="https://github.com/luanma-sys">https://github.com/luanma-sys</a></li>
        </ul>
      </div>
      <div>The source code is available on GitHub:
        <a href="https://github.com/luanma-sys?tab=repositories">https://github.com/luanma-sys?tab=repositories</a>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'


</script>

<style>
.indexhead {
  width: 80%;
  margin: auto;
  height: 270px;

}

.indexbody {
  height: 200px;
  width: 80%;
  margin: auto;

}

.indexfoot {
  height: 170px;
  width: 90%;
  margin: auto;

}
</style>