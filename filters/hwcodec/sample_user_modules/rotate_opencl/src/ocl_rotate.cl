/////////////////////////////////////////////////////////////////////////////////////
//

__kernel void RotateY(__read_only image2d_t src_Y, __write_only image2d_t dst_Y)
{
    int2 coord_src = (int2)(get_global_id(0), get_global_id(1));
    int2 dim = get_image_dim(dst_Y);
    int2 coord_dst = dim  - (int2)(1, 1) - coord_src;

    const sampler_t smp = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE; 

    uint4 pixel = read_imageui(src_Y, smp, coord_src);    
    write_imageui(dst_Y, coord_dst, pixel.wzyx);     
}

/////////////////////////////////////////////////////////////////////////////////////
//

__kernel void RotateUV(__read_only image2d_t src_UV, __write_only image2d_t dst_UV)
{
    int2 coord_src = (int2)(get_global_id(0), get_global_id(1));
    int2 dim = get_image_dim(dst_UV);
    int2 coord_dst = dim - (int2)(1, 1) - coord_src;

    const sampler_t smp = CLK_FILTER_NEAREST | CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE; 

    uint4 pixel = read_imageui(src_UV, smp, coord_src);
    write_imageui(dst_UV, coord_dst, pixel.zwxy);
}