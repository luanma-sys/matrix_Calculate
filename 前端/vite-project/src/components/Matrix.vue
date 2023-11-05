<template>
    <div>
        <table border="1" class="my_matrix">
            <tr v-for="(item, row) in matrix" :key="row">
                <td v-for="(value, col) in item" :key="col" @click="showEditInput($event, row, col)" ref="panel">
                    <div v-if="result"> <input class="my_input" type="text" :value="value"
                            @input="onInput($event, row, col)" @blur="onBlur($event, row, col)" @click="onClick"
                            readonly="readonly" /></div>
                    <div v-else><input class="my_input" type="number" :value="value" @input="onInput($event, row, col)"
                            @blur="onBlur($event, row, col)" @click="onClick" /> </div>
                </td>
            </tr>
        </table>
    </div>
</template>

<script>
export default {
    name: 'matrix'
}
</script>

<script setup>
import { toRefs, ref, onMounted, computed, watch } from 'vue'
import { useStore } from 'vuex'

const store = useStore()
const panel = ref(null)

const props = defineProps({
    data: {
        type: Object,
        default: () => ({
            matrix: {},
            id: 0,
            row: 0,
            col: 0,
            result: true,
        })
    }
});

const emit = defineEmits(['submit']);
const { matrix, result, row, col } = toRefs(props.data);

function showEditInput(event, row, col) {

}

let changemat = computed(() => {
    //对矩阵的行进行增加或减少
    if (row.value > matrix.value.length) {
        var matrow = [];
        for (let i = 0; i < col; i++) { matrow.push("0"); }
        for (let i = 0; i < row.value - matrix.value.length; i++) { matrix.value.push([...matrow]) }
    }
    if (row.value < matrix.value.length) {
        var mattemp = [];
        for (let i = 0; i < row.value; i++) { mattemp.push(matrix.value[i]); }
        matrix.value = mattemp;
    }
})

watch(row, (value, oldValue) => {
    if (row.value > matrix.value.length) {
        var matrow = [];
        for (let i = 0; i < col.value; i++) { matrow.push("0"); }
        for (let i = 0; i < row.value - oldValue; i++) { matrix.value.push([...matrow]) }

    }

    if (row.value < matrix.value.length) {
        var mattemp = [];
        for (let i = 0; i < row.value; i++) { mattemp.push(matrix.value[i]); }
        matrix.value = mattemp;
    }
})
watch(col, (value, oldValue) => {
    if(row.value > 0 && col.value > matrix.value[0].length){
        for(var m_r=0; m_r<row.value;m_r++)
        {
            for(var m_c=oldValue; m_c<value; m_c++)
            { 
                matrix.value[m_r].push("0")
            }
        }
    }

    if(row.value>0 && value<oldValue)
    {
        for(var m_r=0; m_r<row.value;m_r++)
        {
            matrix.value[m_r].splice(value,oldValue);
        }
    }
})


const onInput = (e, row, col) => {
    matrix.value[row][col] = e.target.value
}

const onBlur = (e, row, col) => {
    if (matrix.value[row][col] == "") matrix.value[row][col] = 0;
    if (typeof (matrix.value[row][col]) == "number") { return; }    //如果是数字就不做字符串判断
    if (matrix.value[row][col].indexOf("e") != -1) {
        alert("不能输入字母");
        matrix.value[row][col] = 0;
    }
            //去掉非0数字中的无效0
    if (matrix.value[row][col][0] == "0" && matrix.value[row][col].length > 1) {
        var idx = 0;
        for (idx = 0; idx < matrix.value[row][col].length; idx++) {
            if (matrix.value[row][col][idx] != "0" ) break;
        }
        if (idx == matrix.value[row][col].length) return;
        if(matrix.value[row][col][idx]=='.')    idx--;
        matrix.value[row][col] = matrix.value[row][col].slice(idx, matrix.value[row][col].length)
    }
}

function open() {
    store.commit("openMatMessageSuspend", [row,col])
   // store.commit("openButCol", col)
}

const onClick = (e) => {
    e.stopPropagation();
    if (result.value != true) open();
}//这个函数的作用起下面函数的作用，因为onclick只作用于组件，所以不需要ref

/*


onMounted(()=>{
      document.addEventListener('click',(e)=>{
       if(panel.value){
        console.log(panel.value)
          let isSelf = panel.value.contains(e.target)
           if(isSelf){
            console.log("matrix in")
            //open()
           }
       }
   })
    });
*/
</script>

<style>
.my_matrix {
    border-collapse: collapse;
}

.my_matrix td {
    position: relative;
    text-align: center;
    height: 34px;
    width: 34px;
    cursor: pointer;

}

.my_input {
    position: absolute;

    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    box-sizing: border-box;
    border: 1px solid gray;
    text-align: center;
    outline: none;
}


input::-webkit-outer-spin-button,
input::-webkit-inner-spin-button {
    -webkit-appearance: none;
}
</style>