<template>
    <div>
        <div class="my_result contentmat_noflex">
            <div >
                <div> <span v-for="(eigen, index) in tableDate.return_mat[0].eigen_gather" :key="index">
                        <span v-if="index == 0">&nbsp;特征值:</span> &nbsp;{{ eigen }}&nbsp;</span> </div>
                <div v-for="(mat, index) in tableDate.return_mat" :key="index" class="my_unit contentmat_item "
                    style="white-space: nowrap;">
                    <p class="ans_matrix ans_value">{{ mat.constant }}
                        <span v-if="tableDate.return_mat[0].eigen != null" style="color:orange">*</span>
                    </p>
                    <p class="ans_matrix ans_value">{{ mat.eigen }}
                        <span v-if="tableDate.return_mat[0].eigen != null" style="color:orange">*</span>
                    </p>

                    <Matrix v-if="('matrix' in tableDate.return_mat[0])" :data="mat" class="ans_matrix"></Matrix>

                    <p class="ans_matrix ans_value"><span v-if="tableDate.return_mat[0].eigen != null"
                            style="color:orange">*</span>
                        {{ mat.partial }} </p>
                    <p v-if="index != tableDate.return_mat.length - 1" class="ans_matrix ans_value">&nbsp;&nbsp;+&nbsp;&nbsp;
                    </p>
                </div>
            </div>
            <div> {{ tableDate.return_mat[0].areaOrDimensions }} </div>

            <div style="clear:both">
                <slot></slot>
            </div>
        </div>
    </div>
</template>


<script setup>
import { toRefs } from 'vue'
import Matrix from '../components/Matrix.vue'
import { reactive } from 'vue'

const tableDate = reactive({
    return_mat: [
        {
            id: 1,
            row: 3,
            col: 3,
            matrix: [[0, 0, 0], [0, 0, 0], [0, 0, 0]],
            result: true,
            rank: []
        }
    ],
    return_eig: [],
    return_constant: [],
})

const props = defineProps({
    data: {
        type: Object,
        default: () => ({
            matrix: {},
            id: 0,
            row: 0,
            col: 0,
            return_m: [],
        })
    }
});

const { return_m } = toRefs(props.data);
if (props.data.return_m != undefined) {
    tableDate.return_mat = return_m;
}

</script>

<style>
.my_result {
    background-color: #f5f5f5;
    height: 100%
}

.my_unit {
    float: left;
}

.ans_matrix {
    float: left;
}

.ans_value {
    line-height: 80px;
}

.my_contentmat
 { 
    display: flex;
    overflow: scroll;
    white-space: nowrap;
 }
</style>