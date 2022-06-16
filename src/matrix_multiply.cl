__kernel void matrixMul
(
    __global float* result,
    __global float* first_operand,
    __global float* second_operand,
    unsigned int width_first_operand, 
    unsigned int width_second_operand
)
{
    int tx = get_global_id(0);
    int ty = get_global_id(1);

    float value;

    for(unsigned int k = 0; k < width_first_operand; ++k)
    {
        float elementA = first_operand[ty *  width_first_operand + k];
        float elementB = second_operand[k *  width_second_operand + tx];
        value += elementA * elementB;
    }

    result[ty * width_first_operand + tx] = value;
}