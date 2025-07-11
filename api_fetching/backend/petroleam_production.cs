using Microsoft.AspNetCore.Mvc;
using System.Net.Http;
using System.Threading.Tasks;

namespace PetroleumApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class PetroleumController : ControllerBase
    {
        private readonly IHttpClientFactory _httpClientFactory;

        public PetroleumController(IHttpClientFactory httpClientFactory)
        {
            _httpClientFactory = httpClientFactory;
        }

        [HttpGet]
        public async Task<IActionResult> GetData()
        {
            var apiUrl = "https://api.data.gov.in/resource/8b75d7c2-814b-4eb2-9698-c96d69e5f128?api-key=579b464db66ec23bdd000001cdd3946e44ce4aad7209ff7b23ac571b&format=json&offset=0&limit=1000";
            var client = _httpClientFactory.CreateClient();
            HttpResponseMessage response;
            try
            {
                response = await client.GetAsync(apiUrl);
            }
            catch (HttpRequestException ex)
            {
                return StatusCode(500, $"Error fetching data: {ex.Message}");
            }

            if (!response.IsSuccessStatusCode)
                return StatusCode((int)response.StatusCode, "No Data Found");

            var data = await response.Content.ReadAsStringAsync();
            return Content(data, "application/json");
        }
    }
}


