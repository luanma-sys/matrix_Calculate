import axios from 'axios'
import { mapState } from 'vuex'
import useStore from '../store/index.js'
export default{
    getMatAdd(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_add&value=' + matStr
        })
    },
    getMatMultip(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_multip&value=' + matStr
        })
    },
    getMatECHELON(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_echelon&value=' + matStr
        })
    },
    getMatTranspose(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_transpose&value=' + matStr
        })
    },
    getMatDimensions(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_dimensions&value=' + matStr
        })
    },
    getMatArea(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_area&value=' + matStr
        })
    },
    getMatRankVectors(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_rank_vectors&value=' + matStr
        })
    },
    getMatRank(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=matrix_rank&value=' + matStr
        })
    },
    getLinearRelation(matStr){
        var str = useStore.getters.getBaseMATURL
        return axios({
            method: 'get',
            url: str + '&intername=linearRelation&value=' + matStr
        })
    },
}