module top #(
    parameter int N = 5
) (
    input [N-1:0] mask_in,
    input [N-1:0][31:0] data_in,
    output logic [31:0] data_out
);

    always_comb begin
        data_out = '0;
        for (int i = 0; i < N; i++) begin
            if (mask_in[i]) data_out = data_in[i];
        end
    end

endmodule
