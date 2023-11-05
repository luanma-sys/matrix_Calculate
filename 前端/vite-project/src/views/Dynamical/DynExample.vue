<template>
  <div>
    <div class="helphead">
      <div style="width:900px;float:left;margin-top:65px">
        1990年，在利用或滥用太平洋西北部大面积森林的问题上，北方的猫头鹰成为一个争议的焦点，环境保护学家视图说服联邦政府，
        如果采伐原始森林的行为不得到制止，猫头鹰将濒临灭绝，而木材行业却争辩说猫头鹰不应该被划为濒临灭绝动物，并引用一些已发表的科学报告来支持
        其论点，对木材行业来说，如果政府出台新的伐木限制，预计将失去3万至10万的工作岗位。<br/><p></p>
        生态学家处于争端双方的中间，由于争论的双方都向游说生态学家，生态学家加快了他们对猫头鹰种群的动力学研究，猫头鹰的生命周期自然分为三个阶段：
        幼年期（一岁前），半成年（1-2岁），成年期（2岁后）。猫头鹰在半成年期和成年期交配繁衍，可活到20岁左右，每一对猫头鹰需要约1000公顷的土地作为
        自己的栖息地，生命周期的关键是当幼年猫头鹰离开巢的时候，为了生存和进入半成年期，一只幼年猫头鹰必须成功的找到一个新的栖息地安家
      </div>
      <img src="../../../public/img/bob.jpg" width="400" height="300" style="float:right"/></div>
    <div class="helpbody">
      <div style="width:1200px;margin:auto">
        研究种群动力学的第一步是建立以每年的种群数量为区间的种群模型，时间为k=0,1,2... 通常可以假设在每一个生命阶段雄性和雌性的比例为1:1，而且只计算雌性猫头鹰，
        第k年的种群量可以用向量Xk=(j,s,a)表示，其中j,s和a分别表示雌性猫头鹰在幼年期，半成年期和成年期的数量。
      </div>
      <div>
        利用人口统计研究的实际现场数据，Lamberson及同事设计以下的“阶段矩阵模型”
      </div>
       <div> <img src="../../../public/img/mat.jpg" height="100px"/>  </div>
       <div style="width:1200px;margin:auto">这里新的幼年猫头鹰在k+1年中的数量是承诺雌性猫头鹰在k年数量的0.33倍(根据每一对猫头鹰的平均生殖率而定)。此外，18%的幼年雌性猫头鹰得以生存进入半成年
        期。71%的半成年猫头鹰和94%的承诺雌性猫头鹰生存下来被记为成年猫头鹰
       </div>
       <div style="width:1200px;margin:auto">Lamberson阶段矩阵中，18%的幼年猫头鹰生产率是受原始森林中猫头鹰数目影响最大的项目，事实上，60%的幼年猫头鹰生存下来后就会离开自己的巢，
          但在Lamberson和他的同事们作研究的Willow Creek地区，只有30%的幼年猫头鹰在弃巢后能找到新的栖息地，其余的都在寻找新家园起家失踪
       </div>
    </div>
  </div>
</template>

<script setup>

</script>

<style scoped>
.helphead{
  height: 300px;

}
.helpbody{
  margin-top:30px;
  height: 350px;
  text-align: center;

}
</style>