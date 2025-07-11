using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
 
var builder = WebApplication.CreateBuilder(args);
 
// Add services
builder.Services.AddControllers();
builder.Services.AddHttpClient();
 

builder.Services.AddCors(options =>
{
    options.AddPolicy("AllowFrontend", policy =>
    {
policy.WithOrigins("http://localhost:3000") // React dev server
              .AllowAnyHeader()
              .AllowAnyMethod();
    });
});
 
var app = builder.Build();

app.UseCors("AllowFrontend");
 
app.UseAuthorization();
 
app.MapControllers();

app.Run();
 